using Apsis.Api.Modules.Simulations;
using Apsis.Api.Modules.Users;
using Microsoft.EntityFrameworkCore;

namespace Apsis.Api.Data;

public class ApsisDbContext(DbContextOptions<ApsisDbContext> options) : DbContext(options)
{
    public DbSet<User> Users => Set<User>();
    public DbSet<Simulation> Simulations => Set<Simulation>();

    protected override void OnModelCreating(ModelBuilder modelBuilder)
    {
        modelBuilder.Entity<User>(entity =>
        {
            entity.HasIndex(u => u.Email).IsUnique();
            // Filtered — only Google-linked accounts have a GoogleId, so the index
            // shouldn't reject multiple NULLs the way a plain unique index would.
            entity.HasIndex(u => u.GoogleId).IsUnique().HasFilter("\"GoogleId\" IS NOT NULL");
            entity.Property(u => u.Email).IsRequired();
            entity.Property(u => u.DisplayName).IsRequired();
            entity.Property(u => u.Role).IsRequired();
        });

        modelBuilder.Entity<Simulation>(entity =>
        {
            entity.HasIndex(s => s.OwnerId);
            entity.HasIndex(s => s.ShareToken).IsUnique();
            entity.Property(s => s.Name).IsRequired();
            entity.Property(s => s.InputType).HasConversion<string>().IsRequired();
        });
    }
}
