using System.Security.Claims;
using Apsis.Api.Data;
using Microsoft.AspNetCore.Authorization;
using Microsoft.AspNetCore.Mvc;
using Microsoft.EntityFrameworkCore;

namespace Apsis.Api.Modules.Users;

/// <summary>
/// Product 1 scope: account CRUD + auth only. Org membership fields exist on
/// the user record from day one but stay unused until Product 2 — see
/// docs/products/product-2-mission-planner.md.
/// </summary>
[ApiController]
[Route("api/v1/users")]
[Authorize]
public class UsersController(ApsisDbContext db) : ControllerBase
{
    /// <summary>Whoever the bearer token belongs to — any authenticated role.</summary>
    [HttpGet("me")]
    public async Task<ActionResult<UserResponse>> GetCurrentUser()
    {
        var userId = Guid.Parse(User.FindFirstValue(ClaimTypes.NameIdentifier)
            ?? User.FindFirstValue("sub")!);

        var user = await db.Users.FindAsync(userId);
        if (user is null)
        {
            return NotFound();
        }

        return Ok(UserResponse.FromEntity(user));
    }

    /// <summary>Admin-only: demonstrates role-based authorization on top of the [Authorize] above.</summary>
    [HttpGet]
    [Authorize(Roles = Roles.Admin)]
    public async Task<ActionResult<List<UserResponse>>> ListUsers()
    {
        // SQLite can't translate ORDER BY on DateTimeOffset server-side — sort client-side.
        var users = await db.Users.ToListAsync();
        return Ok(users.OrderBy(u => u.CreatedAt).Select(UserResponse.FromEntity));
    }
}
