namespace Apsis.Api.Modules.Users;

/// <summary>
/// Product 1 scope: account identity only. <see cref="OrgId"/> is unused until
/// Product 2's Modules/Orgs exists — see docs/products/product-2-mission-planner.md.
/// </summary>
public class User
{
    public Guid Id { get; set; } = Guid.NewGuid();

    public required string Email { get; set; }

    /// <summary>Null for accounts created via Google sign-in that never set a local password.</summary>
    public string? PasswordHash { get; set; }

    /// <summary>Google's stable subject id ("sub" claim). Null for email/password-only accounts.</summary>
    public string? GoogleId { get; set; }

    public required string DisplayName { get; set; }

    /// <summary>Coarse role for [Authorize(Roles = ...)] checks. "Admin" or "User".</summary>
    public string Role { get; set; } = Roles.User;

    public Guid? OrgId { get; set; }

    public DateTimeOffset CreatedAt { get; set; } = DateTimeOffset.UtcNow;
}

public static class Roles
{
    public const string Admin = "Admin";
    public const string User = "User";
}
