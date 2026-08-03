var builder = WebApplication.CreateBuilder(args);

// Add services to the container.
// Product 1 stays a modular monolith: every Modules/* folder below is a
// namespace inside this one project, not a separate service. See
// docs/architecture/folder-structure.md before adding a new project/service.

builder.Services.AddControllers();
// Learn more about configuring OpenAPI at https://aka.ms/aspnet/openapi
builder.Services.AddOpenApi();

// TODO(Sprint 3-4): AddDbContext<ApsisDbContext> (Postgres via EF Core).
// TODO(Sprint 3-4): AddAuthentication/AddIdentity (see Modules/Users).

var app = builder.Build();

// Configure the HTTP request pipeline.
if (app.Environment.IsDevelopment())
{
    app.MapOpenApi();
}

app.UseHttpsRedirection();

app.UseAuthorization();

app.MapControllers();

app.MapGet("/health", () => Results.Ok(new { status = "ok" }));

app.Run();
