using Microsoft.AspNetCore.Mvc;

namespace Apsis.Api.Modules.Users;

/// <summary>
/// Product 1 scope: account CRUD + auth only. Org membership fields exist on
/// the user record from day one but stay unused until Product 2 — see
/// docs/products/product-2-mission-planner.md.
/// </summary>
[ApiController]
[Route("api/v1/users")]
public class UsersController : ControllerBase
{
    // TODO(Sprint 3-4): wire up ASP.NET Core Identity, JWT issuance, Google OAuth.
    // See apps/api/README.md for the Sprint plan this controller belongs to.

    [HttpGet("me")]
    public IActionResult GetCurrentUser()
    {
        return Ok(new { message = "Auth not wired up yet — see apps/api/README.md" });
    }
}
