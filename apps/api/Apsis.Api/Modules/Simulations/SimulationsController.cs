using Microsoft.AspNetCore.Mvc;

namespace Apsis.Api.Modules.Simulations;

/// <summary>
/// Product 1 core API — see docs/products/product-1-orbital-simulator.md.
/// Only inputs (TLE/elements, timespan) are persisted; propagated state
/// vectors are recomputed on read, never stored, per that doc's data-flow note.
/// </summary>
[ApiController]
[Route("api/v1/simulations")]
public class SimulationsController : ControllerBase
{
    // TODO(Sprint 3-4): CRUD against Postgres via EF Core.
    [HttpGet]
    public IActionResult List() => Ok(Array.Empty<object>());

    [HttpPost]
    public IActionResult Create() => StatusCode(StatusCodes.Status501NotImplemented);

    // TODO(Sprint 1-2): call into Propagation/Sgp4 (in-process, no gRPC — see ADR-0001).
    [HttpGet("{id:guid}/propagate")]
    public IActionResult Propagate(Guid id) => StatusCode(StatusCodes.Status501NotImplemented);

    // TODO(Sprint 7-8): call into Propagation/Keplerian and diff against Sgp4 output.
    [HttpGet("{id:guid}/compare")]
    public IActionResult Compare(Guid id) => StatusCode(StatusCodes.Status501NotImplemented);

    [HttpGet("{id:guid}/export")]
    public IActionResult Export(Guid id) => StatusCode(StatusCodes.Status501NotImplemented);
}
