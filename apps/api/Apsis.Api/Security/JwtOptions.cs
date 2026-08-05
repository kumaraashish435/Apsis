namespace Apsis.Api.Security;

/// <summary>Bound from the "Jwt" section of appsettings.json / user-secrets / env vars.</summary>
public class JwtOptions
{
    public const string SectionName = "Jwt";

    public required string Issuer { get; set; }
    public required string Audience { get; set; }
    public required string SigningKey { get; set; }
    public int AccessTokenMinutes { get; set; } = 60;
}
