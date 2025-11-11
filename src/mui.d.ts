import '@mui/material/styles';

declare module '@mui/material/styles' {
  interface PaletteOptions {
    chartColors?: string[];
  }
  interface Palette {
    chartColors: string[];
  }
} 