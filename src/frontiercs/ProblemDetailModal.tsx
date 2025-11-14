import React from 'react';
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
} from '@mui/material';
import { MdClose } from 'react-icons/md';

interface ProblemDetailModalProps {
  open: boolean;
  onClose: () => void;
  problem: string;
  problemName: string;
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

export const ProblemDetailModal: React.FC<ProblemDetailModalProps> = ({
  open,
  onClose,
  problem,
  problemName,
}) => {
  const [tabValue, setTabValue] = React.useState(0);

  const handleTabChange = (_event: React.SyntheticEvent, newValue: number) => {
    setTabValue(newValue);
  };

  // Problem statement URL
  const problemStatementUrl = `/problems/${problem}/statement.txt`;
  const groundTruthCodeUrl = `/problems/${problem}/solution.cpp`;

  // Model solution URLs
  const models = ['gpt', 'gemini', 'claude', 'grok'];
  const modelSolutionUrls = models.map(model => ({
    model,
    url: `/solutions/${problem}_0_${model}_solution.cpp`,
    resultUrl: `/solutions/${problem}_0_${model}_result.json`,
  }));

  return (
    <Dialog
      open={open}
      onClose={onClose}
      maxWidth="lg"
      fullWidth
      PaperProps={{
        sx: {
          height: '90vh',
          maxHeight: 800,
        }
      }}
    >
      <DialogTitle sx={{ m: 0, p: 2, bgcolor: 'grey.100', borderBottom: 1, borderColor: 'divider' }}>
        <Stack direction="row" alignItems="center" justifyContent="space-between">
          <Typography variant="h6" component="div">
            {problemName}
          </Typography>
          <IconButton
            aria-label="close"
            onClick={onClose}
            sx={{
              color: (theme) => theme.palette.grey[500],
            }}
          >
            <MdClose />
          </IconButton>
        </Stack>
      </DialogTitle>
      <DialogContent dividers sx={{ p: 0 }}>
        <Box sx={{ borderBottom: 1, borderColor: 'divider' }}>
          <Tabs value={tabValue} onChange={handleTabChange} aria-label="problem detail tabs">
            <Tab label="Problem Statement" />
            <Tab label="LLM Solutions" />
            <Tab label="Ground Truth" />
          </Tabs>
        </Box>

        <TabPanel value={tabValue} index={0}>
          <Paper elevation={0} sx={{ p: 2, bgcolor: 'grey.50', overflow: 'auto', maxHeight: 600 }}>
            <Stack spacing={2}>
              <Typography variant="body2" color="text.secondary">
                View the full problem statement:
              </Typography>
              <Box>
                <a
                  href={problemStatementUrl}
                  target="_blank"
                  rel="noopener noreferrer"
                  style={{
                    color: '#6366f1',
                    textDecoration: 'none',
                    fontWeight: 500,
                  }}
                >
                  Open Problem Statement →
                </a>
              </Box>
              <Typography variant="caption" color="text.secondary">
                Problem file: {problem}/statement.txt
              </Typography>
            </Stack>
          </Paper>
        </TabPanel>

        <TabPanel value={tabValue} index={1}>
          <Stack spacing={2}>
            {modelSolutionUrls.map(({ model, url, resultUrl }) => (
              <Paper key={model} elevation={0} sx={{ p: 2, border: 1, borderColor: 'divider' }}>
                <Stack spacing={1.5}>
                  <Typography variant="subtitle2" sx={{ textTransform: 'capitalize', fontWeight: 600 }}>
                    {model.toUpperCase()} Solution
                  </Typography>
                  <Stack direction="row" spacing={2}>
                    <a
                      href={url}
                      target="_blank"
                      rel="noopener noreferrer"
                      style={{
                        color: '#6366f1',
                        textDecoration: 'none',
                        fontSize: '0.875rem',
                      }}
                    >
                      View Code →
                    </a>
                    <a
                      href={resultUrl}
                      target="_blank"
                      rel="noopener noreferrer"
                      style={{
                        color: '#059669',
                        textDecoration: 'none',
                        fontSize: '0.875rem',
                      }}
                    >
                      View Results →
                    </a>
                  </Stack>
                  <Typography variant="caption" color="text.secondary">
                    Solution file: {problem}_0_{model}_solution.cpp
                  </Typography>
                </Stack>
              </Paper>
            ))}
          </Stack>
        </TabPanel>

        <TabPanel value={tabValue} index={2}>
          <Paper elevation={0} sx={{ p: 2, bgcolor: 'grey.50', overflow: 'auto', maxHeight: 600 }}>
            <Stack spacing={2}>
              <Typography variant="body2" color="text.secondary">
                View the ground truth / reference solution:
              </Typography>
              <Box>
                <a
                  href={groundTruthCodeUrl}
                  target="_blank"
                  rel="noopener noreferrer"
                  style={{
                    color: '#6366f1',
                    textDecoration: 'none',
                    fontWeight: 500,
                  }}
                >
                  Open Ground Truth Code →
                </a>
              </Box>
              <Typography variant="caption" color="text.secondary">
                Ground truth file: {problem}/solution.cpp
              </Typography>
            </Stack>
          </Paper>
        </TabPanel>
      </DialogContent>
    </Dialog>
  );
};
