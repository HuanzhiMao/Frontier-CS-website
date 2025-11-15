import React, { useState, useEffect } from 'react';
import {
  Dialog,
  DialogTitle,
  DialogContent,
  IconButton,
  Box,
  Tabs,
  Tab,
  Typography,
  Paper,
  Stack,
  CircularProgress,
  Chip,
  Divider,
} from '@mui/material';
import { MdClose } from 'react-icons/md';
import { Prism as SyntaxHighlighter } from 'react-syntax-highlighter';
import { vscDarkPlus } from 'react-syntax-highlighter/dist/esm/styles/prism';

interface ProblemDetailModalProps {
  open: boolean;
  onClose: () => void;
  problem: string;
  problemName: string;
  modelScores?: { model: string; score: number }[];
}

interface TabPanelProps {
  children?: React.ReactNode;
  index: number;
  value: number;
}

function TabPanel(props: TabPanelProps) {
  const { children, value, index, ...other } = props;

  return (
    <div
      role="tabpanel"
      hidden={value !== index}
      id={`problem-tabpanel-${index}`}
      aria-labelledby={`problem-tab-${index}`}
      {...other}
    >
      {value === index && <Box sx={{ p: 3 }}>{children}</Box>}
    </div>
  );
}

async function fetchFileContent(url: string): Promise<string> {
  try {
    const response = await fetch(url);
    if (!response.ok) {
      return `File not found (${response.status})`;
    }

    // Check if response is actually text/plain or code
    const contentType = response.headers.get('content-type');
    if (contentType && contentType.includes('text/html')) {
      return 'File not found or invalid file type';
    }

    const text = await response.text();

    // Additional check: if content starts with HTML tags, it's likely a 404 page
    if (text.trim().startsWith('<!DOCTYPE') || text.trim().startsWith('<html')) {
      return 'File not found';
    }

    return text;
  } catch (error) {
    return `Error loading file: ${error}`;
  }
}

export const ProblemDetailModal: React.FC<ProblemDetailModalProps> = ({
  open,
  onClose,
  problem,
  problemName,
  modelScores = [],
}) => {
  const [tabValue, setTabValue] = useState(0);
  const [statement, setStatement] = useState<string>('');
  const [groundTruth, setGroundTruth] = useState<string>('');
  const [llmSolutions, setLlmSolutions] = useState<Map<string, string>>(new Map());
  const [loading, setLoading] = useState(true);

  const handleTabChange = (_event: React.SyntheticEvent, newValue: number) => {
    setTabValue(newValue);
  };

  // Helper function to get score for a model
  const getModelScore = (modelKey: string): number => {
    const modelNameMapping: Record<string, string> = {
      'gpt': 'GPT-5-2025-08-07',
      'gemini': 'Gemini-2.5-Pro',
      'claude': 'Claude-Sonnet-4-5-20250929',
      'grok': 'Grok-Code-Fast-1',
    };

    const fullModelName = modelNameMapping[modelKey];
    const scoreData = modelScores.find(s => s.model.startsWith(fullModelName));
    return scoreData?.score ?? 0;
  };

  // Load files when modal opens
  useEffect(() => {
    if (!open || !problem) return;

    setLoading(true);

    // Load statement
    fetchFileContent(`/problems/${problem}/statement.txt`).then(setStatement);

    // Load ground truth
    fetchFileContent(`/problems/${problem}/solution.cpp`).then(setGroundTruth);

    // Load LLM solutions
    const models = ['gpt', 'gemini', 'claude', 'grok'];
    Promise.all(
      models.map(async (model) => {
        const content = await fetchFileContent(`/solutions/${problem}_0_${model}_solution.cpp`);
        return [model, content] as [string, string];
      })
    ).then((results) => {
      setLlmSolutions(new Map(results));
      setLoading(false);
    });
  }, [open, problem]);

  const models = ['gpt', 'gemini', 'claude', 'grok'];
  const modelNames: Record<string, string> = {
    gpt: 'GPT 5 Thinking',
    gemini: 'Gemini 2.5 Pro',
    claude: 'Claude Sonnet 4.5',
    grok: 'Grok 4',
  };

  // Color themes for each model
  const modelColors: Record<string, { gradient: string; light: string }> = {
    gpt: {
      gradient: 'linear-gradient(135deg, #667eea 0%, #764ba2 100%)',
      light: '#f3e8ff',
    },
    gemini: {
      gradient: 'linear-gradient(135deg, #4facfe 0%, #00f2fe 100%)',
      light: '#dbeafe',
    },
    claude: {
      gradient: 'linear-gradient(135deg, #fa709a 0%, #fee140 100%)',
      light: '#fef3c7',
    },
    grok: {
      gradient: 'linear-gradient(135deg, #43e97b 0%, #38f9d7 100%)',
      light: '#d1fae5',
    },
  };

  return (
    <Dialog
      open={open}
      onClose={onClose}
      maxWidth="lg"
      fullWidth
      PaperProps={{
        sx: {
          height: '90vh',
          maxHeight: 900,
        }
      }}
    >
      <DialogTitle sx={{ m: 0, p: 3, background: 'linear-gradient(135deg, #667eea 0%, #764ba2 100%)', borderBottom: 1, borderColor: 'divider' }}>
        <Stack direction="row" alignItems="center" justifyContent="space-between">
          <Typography variant="h5" component="div" sx={{ fontWeight: 700, color: 'white' }}>
            {problemName}
          </Typography>
          <IconButton
            aria-label="close"
            onClick={onClose}
            sx={{
              color: 'white',
              '&:hover': {
                bgcolor: 'rgba(255, 255, 255, 0.1)',
              },
            }}
          >
            <MdClose />
          </IconButton>
        </Stack>
      </DialogTitle>
      <DialogContent dividers sx={{ p: 0 }}>
        <Box sx={{ borderBottom: 1, borderColor: 'divider', bgcolor: '#fafafa' }}>
          <Tabs
            value={tabValue}
            onChange={handleTabChange}
            aria-label="problem detail tabs"
            sx={{
              '& .MuiTabs-indicator': {
                background: 'linear-gradient(135deg, #667eea 0%, #764ba2 100%)',
                height: 3,
              },
              '& .MuiTab-root': {
                fontWeight: 600,
                fontSize: '0.95rem',
                textTransform: 'none',
                '&.Mui-selected': {
                  color: '#667eea',
                },
              },
            }}
          >
            <Tab label="Problem Statement" />
            <Tab label="LLM Solutions" />
            <Tab label="Human Solution" />
          </Tabs>
        </Box>

        <TabPanel value={tabValue} index={0}>
          {loading ? (
            <Box sx={{ display: 'flex', justifyContent: 'center', p: 4 }}>
              <CircularProgress sx={{ color: '#667eea' }} />
            </Box>
          ) : (
            <Paper elevation={2} sx={{ p: 0, border: 1, borderColor: 'divider', overflow: 'auto', maxHeight: 700, borderRadius: 2 }}>
              <Box sx={{ p: 2.5, background: 'linear-gradient(135deg, #667eea 0%, #764ba2 100%)', borderBottom: 1, borderColor: 'divider' }}>
                <Typography variant="h6" sx={{ fontWeight: 700, color: 'white', fontSize: '1.1rem', letterSpacing: '0.02em' }}>
                  Problem Description
                </Typography>
              </Box>
              <Box sx={{ p: 4, bgcolor: '#ffffff' }}>
                <Box
                  sx={{
                    fontFamily: "'Inter', -apple-system, BlinkMacSystemFont, 'Segoe UI', sans-serif",
                    fontSize: '0.95rem',
                    lineHeight: 1.8,
                    color: '#1e293b',
                    whiteSpace: 'pre-wrap',
                    wordWrap: 'break-word',
                    '& strong': {
                      fontWeight: 700,
                      color: '#0f172a',
                    },
                    bgcolor: '#f8fafc',
                    p: 3,
                    borderRadius: 2,
                    border: '1px solid',
                    borderColor: '#e2e8f0',
                  }}
                >
                  {statement}
                </Box>
              </Box>
            </Paper>
          )}
        </TabPanel>

        <TabPanel value={tabValue} index={1}>
          {loading ? (
            <Box sx={{ display: 'flex', justifyContent: 'center', p: 4 }}>
              <CircularProgress sx={{ color: '#667eea' }} />
            </Box>
          ) : (
            <Stack spacing={3}>
              {models.map((model) => {
                const score = getModelScore(model);
                const colors = modelColors[model];
                return (
                  <Paper key={model} elevation={2} sx={{ border: 1, borderColor: 'divider', overflow: 'hidden', borderRadius: 2 }}>
                    <Box sx={{ p: 2, background: colors.gradient, borderBottom: 1, borderColor: 'divider', display: 'flex', justifyContent: 'space-between', alignItems: 'center' }}>
                      <Typography variant="subtitle2" sx={{ fontWeight: 700, color: 'white', fontSize: '1rem' }}>
                        {modelNames[model]} Solution
                      </Typography>
                      <Chip
                        label={`Score: ${score}`}
                        size="small"
                        sx={{
                          background: 'rgba(255, 255, 255, 0.9)',
                          color: '#1e293b',
                          fontWeight: 700,
                          backdropFilter: 'blur(10px)',
                          border: '1px solid rgba(255, 255, 255, 0.2)',
                        }}
                      />
                    </Box>
                    {llmSolutions.get(model)?.includes('File not found') || llmSolutions.get(model)?.includes('Error loading') ? (
                      <Box sx={{ display: 'flex', justifyContent: 'center', alignItems: 'center', p: 4 }}>
                        <Typography variant="body2" color="text.secondary">
                          No solution available
                        </Typography>
                      </Box>
                    ) : (
                      <Box sx={{ maxHeight: 500, overflow: 'auto' }}>
                        <SyntaxHighlighter
                          language="cpp"
                          style={vscDarkPlus}
                          customStyle={{
                            margin: 0,
                            borderRadius: 0,
                            fontSize: '0.85rem',
                            lineHeight: 1.6,
                          }}
                          showLineNumbers
                          wrapLines
                        >
                          {llmSolutions.get(model) || 'No solution available'}
                        </SyntaxHighlighter>
                      </Box>
                    )}
                  </Paper>
                );
              })}
            </Stack>
          )}
        </TabPanel>

        <TabPanel value={tabValue} index={2}>
          {loading ? (
            <Box sx={{ display: 'flex', justifyContent: 'center', p: 4 }}>
              <CircularProgress sx={{ color: '#667eea' }} />
            </Box>
          ) : groundTruth.includes('File not found') || groundTruth.includes('Error loading') ? (
            <Box sx={{ display: 'flex', justifyContent: 'center', alignItems: 'center', p: 8 }}>
              <Typography variant="body1" color="text.secondary">
                No ground truth solution available for this problem
              </Typography>
            </Box>
          ) : (
            <Paper elevation={2} sx={{ border: 1, borderColor: 'divider', overflow: 'hidden', borderRadius: 2 }}>
              <Box sx={{ p: 2, background: 'linear-gradient(135deg, #667eea 0%, #764ba2 100%)', borderBottom: 1, borderColor: 'divider' }}>
                <Typography variant="subtitle2" sx={{ fontWeight: 700, color: 'white', fontSize: '1rem' }}>
                  Human Solution
                </Typography>
              </Box>
              <Box sx={{ maxHeight: 600, overflow: 'auto' }}>
                <SyntaxHighlighter
                  language="cpp"
                  style={vscDarkPlus}
                  customStyle={{
                    margin: 0,
                    borderRadius: 0,
                    fontSize: '0.85rem',
                    lineHeight: 1.6,
                  }}
                  showLineNumbers
                  wrapLines
                >
                  {groundTruth}
                </SyntaxHighlighter>
              </Box>
            </Paper>
          )}
        </TabPanel>
      </DialogContent>
    </Dialog>
  );
};
