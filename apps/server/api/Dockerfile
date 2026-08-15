# apps/server/api: ASP.NET Core Web API.
#
# Build:  docker build -t apsis-api -f apps/server/api/Dockerfile apps/server/api
# Run:    docker run --rm -p 8080:8080 apsis-api
#         curl http://localhost:8080/weatherforecast

FROM mcr.microsoft.com/dotnet/sdk:10.0 AS build
WORKDIR /src

COPY Apsis.api.csproj .
RUN dotnet restore

COPY . .
RUN dotnet publish -c Release -o /app --no-restore


FROM mcr.microsoft.com/dotnet/aspnet:10.0 AS runtime
WORKDIR /app
COPY --from=build /app .

ENV ASPNETCORE_URLS=http://+:8080
ENV ASPNETCORE_ENVIRONMENT=Production
EXPOSE 8080

ENTRYPOINT ["dotnet", "Apsis.api.dll"]
