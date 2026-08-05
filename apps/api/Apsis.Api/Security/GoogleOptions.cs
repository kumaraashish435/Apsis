namespace Apsis.Api.Security;

/// <summary>
/// Bound from the "Google" config section. <see cref="ClientId"/> is a public
/// OAuth client identifier, not a secret — safe to expose to the frontend (see
/// AuthController's client-id endpoint) — there is no client secret anywhere
/// in this flow because the API only ever verifies a Google-issued ID token,
/// it never exchanges an authorization code itself.
/// </summary>
public class GoogleOptions
{
    public const string SectionName = "Google";

    /// <summary>Empty until configured — callers must treat that as "Google sign-in disabled".</summary>
    public string ClientId { get; set; } = string.Empty;
}
