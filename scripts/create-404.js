import { copyFileSync, existsSync } from 'node:fs';
import { resolve } from 'node:path';

const indexPath = resolve('dist/index.html');
const fallbackPath = resolve('dist/404.html');

if (!existsSync(indexPath)) {
  console.error('[404-copy] dist/index.html not found; did you run the build?');
  process.exit(1);
}

try {
  copyFileSync(indexPath, fallbackPath);
  console.log('[404-copy] Created 404.html fallback for GitHub Pages SPA routing.');
} catch (err) {
  console.error('[404-copy] Failed to create 404.html fallback:', err);
  process.exit(1);
}
