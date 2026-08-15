# apps/web: Angular (SSR) frontend.
#
# Build:  docker build -t apsis-web -f apps/web/Dockerfile apps/web
# Run:    docker run --rm -p 4000:4000 apsis-web
#         open http://localhost:4000

FROM node:22-alpine AS build
WORKDIR /src

RUN corepack enable

COPY package.json pnpm-lock.yaml pnpm-workspace.yaml ./
RUN pnpm install --frozen-lockfile

COPY . .
RUN pnpm build


FROM node:22-alpine AS runtime
WORKDIR /app

RUN corepack enable
ENV NODE_ENV=production

COPY package.json pnpm-lock.yaml pnpm-workspace.yaml ./
RUN pnpm install --prod --frozen-lockfile

COPY --from=build /src/dist ./dist

ENV PORT=4000
EXPOSE 4000

CMD ["node", "dist/Apsis.client/server/server.mjs"]
