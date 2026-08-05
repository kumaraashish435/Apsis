using System.Globalization;
using System.Security.Claims;
using System.Text;
using System.Text.Json;
using Apsis.Api.Data;
using Apsis.Api.Propagation;
using Apsis.Api.Propagation.Keplerian;
using Apsis.Api.Propagation.Sgp4;
using Microsoft.AspNetCore.Authorization;
using Microsoft.AspNetCore.Mvc;
using Microsoft.EntityFrameworkCore;

namespace Apsis.Api.Modules.Simulations;

/// <summary>
/// Product 1 core API — see docs/products/product-1-orbital-simulator.md and
/// this module's README.md for the full design writeup. Only inputs
/// (TLE/elements, propagation window) are persisted; propagated state
/// vectors are recomputed on every read, never stored.
/// </summary>
[ApiController]
[Route("api/v1/simulations")]
[Authorize]
public class SimulationsController(ApsisDbContext db, ISgp4Propagator sgp4, IKeplerianPropagator keplerian) : ControllerBase
{
    // Caps how many points a single propagate/export/compare call can produce —
    // without this, a huge window + tiny step (e.g. 50 years @ 1s) would try to
    // allocate and serialize tens of millions of points from one authenticated
    // request. 20,000 points is generous for a single simulation's view (13+
    // days at 1-minute steps) while keeping worst-case request cost bounded.
    private const int MaxSamplePoints = 20_000;

    [HttpGet]
    public async Task<ActionResult<List<SimulationResponse>>> List()
    {
        var ownerId = GetOwnerId();
        var sims = await db.Simulations.Where(s => s.OwnerId == ownerId).ToListAsync();
        return Ok(sims.OrderByDescending(s => s.CreatedAt).Select(s => SimulationResponse.FromEntity(s, includeShareToken: true)));
    }

    [HttpPost]
    public async Task<ActionResult<SimulationResponse>> Create(SaveSimulationRequest request)
    {
        var validationError = ValidateInput(request);
        if (validationError is not null)
        {
            return BadRequest(new { message = validationError });
        }

        var sim = new Simulation
        {
            OwnerId = GetOwnerId(),
            Name = request.Name.Trim(),
            StartTime = request.StartTime,
            EndTime = request.EndTime,
            StepSeconds = request.StepSeconds,
        };
        ApplyInput(sim, request);

        db.Simulations.Add(sim);
        await db.SaveChangesAsync();

        return CreatedAtAction(nameof(Get), new { id = sim.Id }, SimulationResponse.FromEntity(sim, includeShareToken: true));
    }

    [HttpGet("{id:guid}")]
    public async Task<ActionResult<SimulationResponse>> Get(Guid id)
    {
        var sim = await FindOwned(id);
        if (sim is null)
        {
            return NotFound();
        }
        return Ok(SimulationResponse.FromEntity(sim, includeShareToken: true));
    }

    [HttpPut("{id:guid}")]
    public async Task<ActionResult<SimulationResponse>> Update(Guid id, SaveSimulationRequest request)
    {
        var sim = await FindOwned(id);
        if (sim is null)
        {
            return NotFound();
        }

        var validationError = ValidateInput(request);
        if (validationError is not null)
        {
            return BadRequest(new { message = validationError });
        }

        sim.Name = request.Name.Trim();
        sim.StartTime = request.StartTime;
        sim.EndTime = request.EndTime;
        sim.StepSeconds = request.StepSeconds;
        ApplyInput(sim, request);

        await db.SaveChangesAsync();
        return Ok(SimulationResponse.FromEntity(sim, includeShareToken: true));
    }

    [HttpDelete("{id:guid}")]
    public async Task<IActionResult> Delete(Guid id)
    {
        var sim = await FindOwned(id);
        if (sim is null)
        {
            return NotFound();
        }

        db.Simulations.Remove(sim);
        await db.SaveChangesAsync();
        return NoContent();
    }

    [HttpGet("{id:guid}/propagate")]
    public async Task<ActionResult<PropagateResponse>> Propagate(Guid id)
    {
        var sim = await FindOwned(id);
        return sim is null ? NotFound() : PropagateCore(sim);
    }

    [HttpGet("{id:guid}/compare")]
    public async Task<ActionResult<CompareResponse>> Compare(Guid id)
    {
        var sim = await FindOwned(id);
        return sim is null ? NotFound() : CompareCore(sim);
    }

    [HttpGet("{id:guid}/export")]
    public async Task<IActionResult> Export(Guid id, [FromQuery] string format = "json")
    {
        var sim = await FindOwned(id);
        return sim is null ? NotFound() : ExportCore(sim, format);
    }

    [HttpPost("{id:guid}/share")]
    public async Task<ActionResult<ShareResponse>> Share(Guid id)
    {
        var sim = await FindOwned(id);
        if (sim is null)
        {
            return NotFound();
        }
        sim.IsPublic = true;
        await db.SaveChangesAsync();
        return Ok(new ShareResponse { IsPublic = true, ShareToken = sim.ShareToken });
    }

    [HttpDelete("{id:guid}/share")]
    public async Task<ActionResult<ShareResponse>> Unshare(Guid id)
    {
        var sim = await FindOwned(id);
        if (sim is null)
        {
            return NotFound();
        }
        sim.IsPublic = false;
        await db.SaveChangesAsync();
        return Ok(new ShareResponse { IsPublic = false, ShareToken = sim.ShareToken });
    }

    // ---- Public, unauthenticated, read-only — the "shareable read-only link" ----

    [HttpGet("shared/{token:guid}")]
    [AllowAnonymous]
    public async Task<ActionResult<SimulationResponse>> GetShared(Guid token)
    {
        var sim = await FindPublic(token);
        return sim is null ? NotFound() : Ok(SimulationResponse.FromEntity(sim, includeShareToken: false));
    }

    [HttpGet("shared/{token:guid}/propagate")]
    [AllowAnonymous]
    public async Task<ActionResult<PropagateResponse>> PropagateShared(Guid token)
    {
        var sim = await FindPublic(token);
        return sim is null ? NotFound() : PropagateCore(sim);
    }

    [HttpGet("shared/{token:guid}/compare")]
    [AllowAnonymous]
    public async Task<ActionResult<CompareResponse>> CompareShared(Guid token)
    {
        var sim = await FindPublic(token);
        return sim is null ? NotFound() : CompareCore(sim);
    }

    [HttpGet("shared/{token:guid}/export")]
    [AllowAnonymous]
    public async Task<IActionResult> ExportShared(Guid token, [FromQuery] string format = "json")
    {
        var sim = await FindPublic(token);
        return sim is null ? NotFound() : ExportCore(sim, format);
    }

    // ---- shared logic ----

    private ActionResult<PropagateResponse> PropagateCore(Simulation sim)
    {
        var (states, error) = TryPropagate(sim);
        if (error is not null)
        {
            return BadRequest(new { message = error });
        }

        return Ok(new PropagateResponse
        {
            States = states!.Select(s => StateVectorResponse.FromStateVector(s, sim.StartTime)).ToList(),
            Elements = states!.Select(OrbitalElementsCalculator.FromStateVector).ToList(),
        });
    }

    private ActionResult<CompareResponse> CompareCore(Simulation sim)
    {
        if (sim.InputType != SimulationInputType.Tle)
        {
            return BadRequest(new
            {
                message = "Comparison requires a TLE-based simulation — SGP4 needs a TLE, and there's no " +
                           "correctness-preserving way to synthesize one from classical elements alone (a real " +
                           "TLE also encodes a drag term the elements don't have).",
            });
        }

        IReadOnlyList<StateVector> sgp4States;
        try
        {
            sgp4States = sgp4.Propagate(sim.TleLine1!, sim.TleLine2!, sim.StartTime, sim.EndTime, TimeSpan.FromSeconds(sim.StepSeconds));
        }
        catch (Sgp4PropagationException ex)
        {
            return BadRequest(new { message = $"SGP4 propagation stopped: {ex.Message}" });
        }

        if (sgp4States.Count == 0)
        {
            return BadRequest(new { message = "Propagation window produced no points." });
        }

        // Seed the Keplerian model with the osculating elements at SGP4's first
        // point, so both series start from "the same orbit" — the divergence
        // shown is purely what SGP4's perturbation terms add over time, not an
        // artifact of comparing two unrelated orbits.
        var seed = OrbitalElementsCalculator.FromStateVector(sgp4States[0]);
        var keplerianElements = new OrbitalElements(
            seed.SemiMajorAxisKm, seed.Eccentricity, seed.InclinationDeg,
            seed.RaanDeg, seed.ArgPerigeeDeg, seed.TrueAnomalyDeg);
        var keplerianStates = keplerian.Propagate(keplerianElements, sim.StartTime, sim.EndTime, TimeSpan.FromSeconds(sim.StepSeconds));

        var count = Math.Min(sgp4States.Count, keplerianStates.Count);
        var divergence = new List<DivergencePoint>(count);
        for (var i = 0; i < count; i++)
        {
            var a = sgp4States[i];
            var b = keplerianStates[i];
            var distanceKm = Math.Sqrt(Math.Pow(a.X - b.X, 2) + Math.Pow(a.Y - b.Y, 2) + Math.Pow(a.Z - b.Z, 2));
            divergence.Add(new DivergencePoint { EpochSeconds = (a.Epoch - sim.StartTime).TotalSeconds, DistanceKm = distanceKm });
        }

        return Ok(new CompareResponse
        {
            Sgp4States = sgp4States.Select(s => StateVectorResponse.FromStateVector(s, sim.StartTime)).ToList(),
            KeplerianStates = keplerianStates.Select(s => StateVectorResponse.FromStateVector(s, sim.StartTime)).ToList(),
            Divergence = divergence,
        });
    }

    private IActionResult ExportCore(Simulation sim, string format)
    {
        var (states, error) = TryPropagate(sim);
        if (error is not null)
        {
            return BadRequest(new { message = error });
        }

        var responses = states!.Select(s => StateVectorResponse.FromStateVector(s, sim.StartTime)).ToList();
        var fileNameBase = SanitizeFileName(sim.Name);

        return format.ToLowerInvariant() switch
        {
            "csv" => File(Encoding.UTF8.GetBytes(BuildCsv(responses)), "text/csv", $"{fileNameBase}.csv"),
            "json" => File(JsonSerializer.SerializeToUtf8Bytes(responses), "application/json", $"{fileNameBase}.json"),
            "czml" => File(Encoding.UTF8.GetBytes(BuildCzml(sim, responses)), "application/json", $"{fileNameBase}.czml"),
            _ => BadRequest(new { message = "format must be one of: csv, json, czml." }),
        };
    }

    private (IReadOnlyList<StateVector>? States, string? Error) TryPropagate(Simulation sim)
    {
        var step = TimeSpan.FromSeconds(sim.StepSeconds);

        if (sim.InputType == SimulationInputType.Tle)
        {
            try
            {
                return (sgp4.Propagate(sim.TleLine1!, sim.TleLine2!, sim.StartTime, sim.EndTime, step), null);
            }
            catch (Sgp4PropagationException ex)
            {
                return (null, $"Propagation stopped: {ex.Message}");
            }
        }

        var elements = new OrbitalElements(
            sim.SemiMajorAxisKm!.Value, sim.Eccentricity!.Value, sim.InclinationDeg!.Value,
            sim.RaanDeg!.Value, sim.ArgPerigeeDeg!.Value, sim.TrueAnomalyDeg!.Value);
        return (keplerian.Propagate(elements, sim.StartTime, sim.EndTime, step), null);
    }

    private static void ApplyInput(Simulation sim, SaveSimulationRequest request)
    {
        sim.InputType = request.InputType;
        sim.TleLine1 = request.InputType == SimulationInputType.Tle ? request.TleLine1!.Trim() : null;
        sim.TleLine2 = request.InputType == SimulationInputType.Tle ? request.TleLine2!.Trim() : null;
        sim.SemiMajorAxisKm = request.InputType == SimulationInputType.Elements ? request.SemiMajorAxisKm : null;
        sim.Eccentricity = request.InputType == SimulationInputType.Elements ? request.Eccentricity : null;
        sim.InclinationDeg = request.InputType == SimulationInputType.Elements ? request.InclinationDeg : null;
        sim.RaanDeg = request.InputType == SimulationInputType.Elements ? request.RaanDeg : null;
        sim.ArgPerigeeDeg = request.InputType == SimulationInputType.Elements ? request.ArgPerigeeDeg : null;
        sim.TrueAnomalyDeg = request.InputType == SimulationInputType.Elements ? request.TrueAnomalyDeg : null;
    }

    private static string? ValidateInput(SaveSimulationRequest r)
    {
        var spanSeconds = (r.EndTime - r.StartTime).TotalSeconds;
        if (spanSeconds < 0)
        {
            return "endTime must not be before startTime.";
        }
        if (spanSeconds / r.StepSeconds > MaxSamplePoints)
        {
            return $"Requested range produces more than {MaxSamplePoints} sample points — increase stepSeconds or shorten the window.";
        }

        if (r.InputType == SimulationInputType.Tle)
        {
            if (string.IsNullOrWhiteSpace(r.TleLine1) || string.IsNullOrWhiteSpace(r.TleLine2))
            {
                return "tleLine1 and tleLine2 are required for a TLE-based simulation.";
            }
            try
            {
                var tle = new TLE(r.TleLine1, r.TleLine2);
                if (!string.IsNullOrEmpty(tle.getParseErrors()))
                {
                    return $"Invalid TLE: {tle.getParseErrors()}";
                }
            }
            catch (Exception ex)
            {
                return $"Invalid TLE: {ex.Message}";
            }
        }
        else
        {
            if (r.SemiMajorAxisKm is not > 0)
            {
                return "semiMajorAxisKm must be positive.";
            }
            if (r.Eccentricity is null || r.Eccentricity is < 0 or >= 1)
            {
                return "eccentricity must be in [0, 1).";
            }
            if (r.InclinationDeg is null || r.InclinationDeg is < 0 or > 180)
            {
                return "inclinationDeg must be in [0, 180].";
            }
            if (r.RaanDeg is null || r.ArgPerigeeDeg is null || r.TrueAnomalyDeg is null)
            {
                return "raanDeg, argPerigeeDeg, and trueAnomalyDeg are required for an elements-based simulation.";
            }
        }

        return null;
    }

    private Guid GetOwnerId()
        => Guid.Parse(User.FindFirstValue(ClaimTypes.NameIdentifier) ?? User.FindFirstValue("sub")!);

    private async Task<Simulation?> FindOwned(Guid id)
    {
        var sim = await db.Simulations.FindAsync(id);
        return sim is not null && sim.OwnerId == GetOwnerId() ? sim : null;
    }

    private async Task<Simulation?> FindPublic(Guid token)
        => await db.Simulations.SingleOrDefaultAsync(s => s.ShareToken == token && s.IsPublic);

    private static string SanitizeFileName(string name)
    {
        var cleaned = new string(name.Where(c => char.IsLetterOrDigit(c) || c is '-' or '_').ToArray()).ToLowerInvariant();
        return string.IsNullOrEmpty(cleaned) ? "simulation" : cleaned;
    }

    private static string BuildCsv(IReadOnlyList<StateVectorResponse> states)
    {
        var sb = new StringBuilder();
        sb.AppendLine("epochSeconds,xKm,yKm,zKm,vxKmS,vyKmS,vzKmS,latDeg,lonDeg");
        foreach (var s in states)
        {
            sb.AppendLine(string.Join(',',
                Inv(s.EpochSeconds), Inv(s.XKm), Inv(s.YKm), Inv(s.ZKm),
                Inv(s.VxKmS), Inv(s.VyKmS), Inv(s.VzKmS), Inv(s.LatDeg), Inv(s.LonDeg)));
        }
        return sb.ToString();

        static string Inv(double d) => d.ToString(CultureInfo.InvariantCulture);
    }

    /// <summary>
    /// A minimal, valid CZML document (position + point + path) — enough for
    /// Cesium or any other CZML-reading tool to render the propagated orbit,
    /// even though this app's own 3D viewer is Three.js, not Cesium. Positions
    /// use referenceFrame "INERTIAL" (CZML's term for a non-Earth-fixed frame)
    /// since SGP4/Keplerian output is ECI/TEME, not Earth-fixed — the honest
    /// choice given this app doesn't do full ICRF/J2000 frame bookkeeping.
    /// </summary>
    private static string BuildCzml(Simulation sim, IReadOnlyList<StateVectorResponse> states)
    {
        var epochIso = sim.StartTime.UtcDateTime.ToString("o", CultureInfo.InvariantCulture);
        var cartesian = new List<double>(states.Count * 4);
        foreach (var s in states)
        {
            cartesian.Add(s.EpochSeconds);
            cartesian.Add(s.XKm * 1000.0);
            cartesian.Add(s.YKm * 1000.0);
            cartesian.Add(s.ZKm * 1000.0);
        }

        object[] document =
        [
            new { id = "document", name = sim.Name, version = "1.0" },
            new
            {
                id = "satellite",
                name = sim.Name,
                position = new { epoch = epochIso, referenceFrame = "INERTIAL", cartesian },
                point = new { pixelSize = 8, color = new { rgba = new[] { 255, 204, 0, 255 } } },
                path = new
                {
                    width = 1,
                    resolution = 120,
                    material = new { solidColor = new { color = new { rgba = new[] { 56, 189, 248, 160 } } } },
                },
            },
        ];

        return JsonSerializer.Serialize(document);
    }
}
