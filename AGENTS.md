# Repository Guidelines

## Project Structure & Module Organization
- The Vite + React + TypeScript app bootstraps from `src/main.tsx`; `src/App.tsx` registers routes via React Router.
- Shared layout components live in `src/layout`, theming in `src/theme.ts`, and leaderboard utilities in `src/leaderboard`.
- Domain pages sit under `src/projects`, `src/overview`, `src/blog`, and `src/problem`, keeping marketing content separate from benchmarking dashboards.
- REST API client stubs in `src/api` are generated files—refresh them with `pnpm generate-client` whenever the backend schema changes.
- Static assets remain in `public/`; build artifacts emit to `dist/`. `scripts/create-404.js` runs post-build to add an SPA-friendly 404 page.

## Build, Test, and Development Commands
- `pnpm install` prepares dependencies.
- `pnpm dev` launches Vite with HMR at `http://localhost:5173`.
- `pnpm build` runs `tsc -b` and `vite build`; follow with `pnpm preview` to inspect the optimized bundle.
- `pnpm lint` executes ESLint using `eslint.config.js`.
- `pnpm generate-client` fetches the OpenAPI spec and rewrites `src/api`—commit the regenerated files only when the schema truly changes.

## Coding Style & Naming Conventions
- Write TypeScript React function components; colocate files inside the feature folder (`projects/<slug>/Component.tsx`).
- Use PascalCase for components, camelCase for hooks/helpers, and kebab-case for directories.
- Match the existing two-space indentation, single quotes, and trailing commas; let ESLint enforce consistency (`pnpm lint --fix`).
- Extend Material UI styling through `theme.ts` and component overrides instead of inline CSS.

## Testing Guidelines
- Automated tests are not set up yet; plan to adopt `vitest` with `@testing-library/react` for UI coverage as you add features.
- Place specs alongside components as `Component.test.tsx` or mirror the path in `src/__tests__`.
- Prioritize scenarios covering routing, leaderboard data states, and API client integration. Document manual QA steps in the PR until suites exist.

## Commit & Pull Request Guidelines
- Keep commits short and imperative; recent history mixes `fix:`-style labels with plain verbs—default to Conventional Commit prefixes (`feat:`, `fix:`, `chore:`) for clarity.
- One logical change per commit; avoid bundling refactors with functional updates.
- PRs should summarize the change, list verification steps, attach UI screenshots when visible changes occur, and link to tracking issues.
- Double-check that generated outputs (`src/api`) and build artifacts stay out of the diff; update `.gitignore` if new tooling appears.
- Request review for modifications touching shared layout or theme modules to maintain design consistency.
