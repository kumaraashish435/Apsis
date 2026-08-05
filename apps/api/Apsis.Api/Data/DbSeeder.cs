using Apsis.Api.Modules.Users;
using Apsis.Api.Security;
using Microsoft.EntityFrameworkCore;

namespace Apsis.Api.Data;

/// <summary>
/// Creates the schema (EnsureCreated — no migrations yet, see apps/api/README.md)
/// and a default admin account so the sign-in page has something to log into
/// on a fresh checkout, without anyone having to hand-seed a database.
/// </summary>
public static class DbSeeder
{
    public const string DefaultAdminEmail = "admin@apsis.dev";
    public const string DefaultAdminPassword = "Apsis#2026!";

    public static async Task SeedAsync(ApsisDbContext db)
    {
        await db.Database.EnsureCreatedAsync();

        if (await db.Users.AnyAsync(u => u.Email == DefaultAdminEmail))
        {
            return;
        }

        db.Users.Add(new User
        {
            Email = DefaultAdminEmail,
            DisplayName = "Apsis Admin",
            PasswordHash = PasswordHasher.Hash(DefaultAdminPassword),
            Role = Roles.Admin,
        });

        await db.SaveChangesAsync();
    }
}
