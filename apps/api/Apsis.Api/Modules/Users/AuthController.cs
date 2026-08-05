using Apsis.Api.Data;
using Apsis.Api.Security;
using Google.Apis.Auth;
using Microsoft.AspNetCore.Mvc;
using Microsoft.EntityFrameworkCore;
using Microsoft.Extensions.Options;

namespace Apsis.Api.Modules.Users;

/// <summary>
/// Sign-up / sign-in, both email/password and Google. Issues JWTs; see
/// Security/JwtTokenService.cs for how they're signed and Program.cs for how
/// they're validated on every other request. See
/// apps/api/Apsis.Api/Modules/Users/README.md for the full design writeup.
/// </summary>
[ApiController]
[Route("api/v1/auth")]
public class AuthController(ApsisDbContext db, JwtTokenService tokenService, IOptions<GoogleOptions> googleOptions)
    : ControllerBase
{
    [HttpPost("register")]
    public async Task<ActionResult<AuthResponse>> Register(RegisterRequest request)
    {
        var normalizedEmail = request.Email.Trim().ToLowerInvariant();

        if (await db.Users.AnyAsync(u => u.Email == normalizedEmail))
        {
            return Conflict(new { message = "An account with this email already exists." });
        }

        var user = new User
        {
            Email = normalizedEmail,
            DisplayName = request.DisplayName.Trim(),
            PasswordHash = PasswordHasher.Hash(request.Password),
            Role = Roles.User,
        };

        db.Users.Add(user);
        await db.SaveChangesAsync();

        return Ok(new AuthResponse
        {
            AccessToken = tokenService.IssueToken(user),
            User = UserResponse.FromEntity(user),
        });
    }

    [HttpPost("login")]
    public async Task<ActionResult<AuthResponse>> Login(LoginRequest request)
    {
        var normalizedEmail = request.Email.Trim().ToLowerInvariant();
        var user = await db.Users.SingleOrDefaultAsync(u => u.Email == normalizedEmail);

        // user.PasswordHash is null for accounts that only ever signed in via Google —
        // there's nothing to verify against, so treat it the same as "wrong password".
        if (user?.PasswordHash is null || !PasswordHasher.Verify(request.Password, user.PasswordHash))
        {
            // Same message for "no such user" and "wrong password" — don't leak which one it was.
            return Unauthorized(new { message = "Invalid email or password." });
        }

        return Ok(new AuthResponse
        {
            AccessToken = tokenService.IssueToken(user),
            User = UserResponse.FromEntity(user),
        });
    }

    /// <summary>
    /// Public OAuth client id, not a secret — the frontend fetches this instead
    /// of hardcoding it so there's one source of truth (this API's config).
    /// Empty string means Google sign-in hasn't been configured; the frontend
    /// hides the button rather than rendering one that can never work.
    /// </summary>
    [HttpGet("google/client-id")]
    public ActionResult<GoogleClientIdResponse> GoogleClientId()
        => Ok(new GoogleClientIdResponse { ClientId = googleOptions.Value.ClientId });

    [HttpPost("google")]
    public async Task<ActionResult<AuthResponse>> GoogleSignIn(GoogleAuthRequest request)
    {
        if (string.IsNullOrEmpty(googleOptions.Value.ClientId))
        {
            return StatusCode(StatusCodes.Status501NotImplemented, new { message = "Google sign-in isn't configured." });
        }

        GoogleJsonWebSignature.Payload payload;
        try
        {
            payload = await GoogleJsonWebSignature.ValidateAsync(request.IdToken, new GoogleJsonWebSignature.ValidationSettings
            {
                Audience = [googleOptions.Value.ClientId],
            });
        }
        catch (InvalidJwtException)
        {
            return Unauthorized(new { message = "Invalid Google credential." });
        }

        var normalizedEmail = payload.Email.Trim().ToLowerInvariant();
        var user = await db.Users.SingleOrDefaultAsync(u => u.GoogleId == payload.Subject || u.Email == normalizedEmail);

        if (user is null)
        {
            user = new User
            {
                Email = normalizedEmail,
                DisplayName = string.IsNullOrWhiteSpace(payload.Name) ? normalizedEmail : payload.Name,
                GoogleId = payload.Subject,
                Role = Roles.User,
            };
            db.Users.Add(user);
        }
        else if (user.GoogleId is null)
        {
            // An email/password account signing in with Google for the first time — link it
            // rather than creating a duplicate account for the same email.
            user.GoogleId = payload.Subject;
        }

        await db.SaveChangesAsync();

        return Ok(new AuthResponse
        {
            AccessToken = tokenService.IssueToken(user),
            User = UserResponse.FromEntity(user),
        });
    }
}
