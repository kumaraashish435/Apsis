using System.Text;
using System.Text.Json.Serialization;
using Apsis.Api.Data;
using Apsis.Api.Propagation.Keplerian;
using Apsis.Api.Propagation.Sgp4;
using Apsis.Api.Security;
using Microsoft.AspNetCore.Authentication.JwtBearer;
using Microsoft.EntityFrameworkCore;
using Microsoft.IdentityModel.Tokens;

var builder = WebApplication.CreateBuilder(args);

// Add services to the container.
// Product 1 stays a modular monolith: every Modules/* folder below is a
// namespace inside this one project, not a separate service. See
// docs/architecture/folder-structure.md before adding a new project/service.

builder.Services.AddControllers()
    // Enums as "Tle"/"Elements" strings over the wire, not magic 0/1 integers —
    // readable in requests/responses and in the SQLite column (see ApsisDbContext's
    // HasConversion<string>() on Simulation.InputType, same reasoning).
    .AddJsonOptions(options => options.JsonSerializerOptions.Converters.Add(new JsonStringEnumConverter()));
// Learn more about configuring OpenAPI at https://aka.ms/aspnet/openapi
builder.Services.AddOpenApi();

// SQLite for now (zero local setup) — swap the provider + connection string
// for Npgsql when Postgres infra lands; see Modules/Users/README.md.
builder.Services.AddDbContext<ApsisDbContext>(options =>
    options.UseSqlite(builder.Configuration.GetConnectionString("Default")));

builder.Services.Configure<JwtOptions>(builder.Configuration.GetSection(JwtOptions.SectionName));
builder.Services.AddSingleton<JwtTokenService>();

builder.Services.Configure<GoogleOptions>(builder.Configuration.GetSection(GoogleOptions.SectionName));

builder.Services.AddSingleton<ISgp4Propagator, Sgp4Propagator>();
builder.Services.AddSingleton<IKeplerianPropagator, KeplerianPropagator>();

var jwtOptions = builder.Configuration.GetSection(JwtOptions.SectionName).Get<JwtOptions>()
    ?? throw new InvalidOperationException("Missing \"Jwt\" configuration section.");

builder.Services
    .AddAuthentication(JwtBearerDefaults.AuthenticationScheme)
    .AddJwtBearer(options =>
    {
        options.TokenValidationParameters = new TokenValidationParameters
        {
            ValidateIssuer = true,
            ValidIssuer = jwtOptions.Issuer,
            ValidateAudience = true,
            ValidAudience = jwtOptions.Audience,
            ValidateLifetime = true,
            ValidateIssuerSigningKey = true,
            IssuerSigningKey = new SymmetricSecurityKey(Encoding.UTF8.GetBytes(jwtOptions.SigningKey)),
            ClockSkew = TimeSpan.FromSeconds(30),
        };
    });

builder.Services.AddAuthorization();

const string CorsPolicy = "Frontend";
var allowedOrigins = builder.Configuration.GetSection("Cors:AllowedOrigins").Get<string[]>() ?? [];
builder.Services.AddCors(options =>
{
    options.AddPolicy(CorsPolicy, policy =>
        policy.WithOrigins(allowedOrigins).AllowAnyHeader().AllowAnyMethod());
});

var app = builder.Build();

// Configure the HTTP request pipeline.
if (app.Environment.IsDevelopment())
{
    app.MapOpenApi();
}

app.UseHttpsRedirection();

app.UseCors(CorsPolicy);

app.UseAuthentication();
app.UseAuthorization();

app.MapControllers();

app.MapGet("/health", () => Results.Ok(new { status = "ok" }));

using (var scope = app.Services.CreateScope())
{
    var db = scope.ServiceProvider.GetRequiredService<ApsisDbContext>();
    await DbSeeder.SeedAsync(db);
}

app.Run();
