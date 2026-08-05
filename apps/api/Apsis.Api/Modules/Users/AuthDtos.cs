using System.ComponentModel.DataAnnotations;

namespace Apsis.Api.Modules.Users;

public record RegisterRequest
{
    [Required, EmailAddress]
    public required string Email { get; init; }

    [Required, MinLength(8, ErrorMessage = "Password must be at least 8 characters.")]
    public required string Password { get; init; }

    [Required, MinLength(1)]
    public required string DisplayName { get; init; }
}

public record LoginRequest
{
    [Required, EmailAddress]
    public required string Email { get; init; }

    [Required]
    public required string Password { get; init; }
}

public record GoogleAuthRequest
{
    /// <summary>The credential string Google Identity Services hands back to the frontend — a signed ID token, not an authorization code.</summary>
    [Required]
    public required string IdToken { get; init; }
}

public record GoogleClientIdResponse
{
    /// <summary>Empty when Google sign-in isn't configured — the frontend hides the button in that case.</summary>
    public required string ClientId { get; init; }
}

public record AuthResponse
{
    public required string AccessToken { get; init; }
    public required UserResponse User { get; init; }
}

public record UserResponse
{
    public required Guid Id { get; init; }
    public required string Email { get; init; }
    public required string DisplayName { get; init; }
    public required string Role { get; init; }

    public static UserResponse FromEntity(User user) => new()
    {
        Id = user.Id,
        Email = user.Email,
        DisplayName = user.DisplayName,
        Role = user.Role,
    };
}
