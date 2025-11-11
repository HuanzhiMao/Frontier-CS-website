import { createTheme, responsiveFontSizes } from '@mui/material/styles';

// Define our color palette
const primaryColor = '#007AFF'; // A modern blue
const secondaryColor = '#5856D6'; // A complementary purple for accents or charts
const backgroundColorPaper = '#FFFFFF';
const backgroundColorDefault = '#F4F6F8'; // Light gray for main background
const textColorPrimary = '#172B4D'; // Dark gray for primary text
const textColorSecondary = '#505F79'; // Lighter gray for secondary text
const borderColor = '#DFE1E6'; // Subtle light gray for borders

let theme = createTheme({
  palette: {
    primary: {
      main: primaryColor,
    },
    secondary: {
      main: secondaryColor,
    },
    background: {
      default: backgroundColorDefault,
      paper: backgroundColorPaper,
    },
    text: {
      primary: textColorPrimary,
      secondary: textColorSecondary,
    },
    divider: borderColor,
    success: {
      main: '#36B37E', // Vibrant Green
    },
    error: {
      main: '#FF5630', // Clear Red/Orange
    },
    // Add a dedicated array for chart colors
    chartColors: [
      '#007AFF', // primary.main
      '#5856D6', // secondary.main
      '#36B37E', // success.main
      '#FFAB00', // A vibrant yellow/orange
      '#FF5630', // error.main (use with caution for data series)
      '#00B8D9', // A nice cyan
      '#6554C0', // Another purple
      '#FF8B00', // Orange
      '#00A3BF', // Darker cyan
      '#3A4856', // A dark neutral for contrast if needed
    ],
  },
  typography: {
    fontFamily: 'Roboto, Arial, sans-serif',
    h1: {
      fontWeight: 700,
      fontSize: '2.5rem', // Example size
    },
    h2: {
      fontWeight: 700,
      fontSize: '2rem',
    },
    h3: {
      fontWeight: 500,
      fontSize: '1.75rem',
    },
    h4: {
      fontWeight: 500,
      fontSize: '1.5rem',
    },
    h5: {
      fontWeight: 500,
      fontSize: '1.25rem',
    },
    h6: {
      fontWeight: 500,
      fontSize: '1.1rem',
    },
    body1: {
      fontWeight: 400,
      lineHeight: 1.5,
    },
    body2: {
      fontWeight: 400,
      lineHeight: 1.4,
      color: textColorSecondary,
    },
    button: {
      fontWeight: 500,
      textTransform: 'none', // Keeping it 'none' for now, can be overridden per component
    },
  },
  shape: {
    borderRadius: 8, // Slightly more rounded corners for a modern feel
  },
  components: {
    MuiPaper: {
      styleOverrides: {
        root: {
          boxShadow: '0px 4px 12px rgba(0, 0, 0, 0.05)', // Softer, more modern shadow
        },
      },
      defaultProps: {
        elevation: 0, // We'll use custom shadows or elevation as needed
      }
    },
    MuiButton: {
      styleOverrides: {
        root: {
          borderRadius: 6, // Consistent with global borderRadius
          padding: '8px 16px',
        },
        containedPrimary: {
          color: '#fff', // Ensure good contrast for primary buttons
        },
      },
    },
    // @ts-ignore
    MuiDataGrid: {
        styleOverrides: {
            root: {
                border: `1px solid ${borderColor}`,
                borderRadius: 8, // Match paper
                '& .MuiDataGrid-cell': {
                    borderBottom: `1px solid ${borderColor}`,
                },
                '& .MuiDataGrid-columnHeaders': {
                    backgroundColor: backgroundColorDefault, // Light header background
                    borderBottom: `1px solid ${borderColor}`,
                    fontWeight: 500,
                },
                '& .MuiDataGrid-columnHeaderTitle': {
                    fontWeight: 'bold', // Make header titles bolder
                },
                 '& .MuiDataGrid-footerContainer': {
                    borderTop: `1px solid ${borderColor}`,
                }
            }
        }
    },
    MuiTab: {
        styleOverrides: {
            root: {
                textTransform: 'none',
                fontWeight: 500,
            }
        }
    },
    MuiTabs: {
        styleOverrides: {
            indicator: {
                height: '3px',
                borderRadius: '3px 3px 0 0',
            }
        }
    },
    MuiChip: {
        styleOverrides: {
            root: {
                fontWeight: 500,
            }
        }
    }
  },
});

// Apply responsive font sizes
theme = responsiveFontSizes(theme);

export default theme; 