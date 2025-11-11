import React from 'react';
import { Box, Button, Container, Stack, Typography } from '@mui/material';

const codeBlockSx = {
  bgcolor: 'grey.900',
  color: 'grey.100',
  fontFamily: 'var(--font-family-mono, "Roboto Mono", monospace)',
  fontSize: 14,
  px: 2,
  py: 1.5,
  borderRadius: 1,
  overflowX: 'auto',
  lineHeight: 1.6,
};

export const LivecodeLocalEvaluationBlog: React.FC = () => {
  return (
    <Box component="article" sx={{ bgcolor: 'background.default', color: 'text.primary' }}>
      <Container maxWidth="md" sx={{ pt: 0, pb: { xs: 6, md: 8 } }}>
        <Stack spacing={4}>
          <Stack spacing={2} sx={{ alignItems: { xs: 'stretch', sm: 'flex-start' } }}>
            <Typography variant="h4" sx={{ fontWeight: 700, fontSize: { xs: '1.5rem', md: '1.75rem' } }}>
              LiveCodeBench Pro Benchmarking Toolkit
            </Typography>
            <Typography variant="body1" sx={{ color: 'text.secondary' }}>
              Everything you need to reproduce LiveCodeBench Pro evaluations locally and plug in your own
              LLM interface.
            </Typography>
            <Button
              component="a"
              href="https://github.com/GavinZhengOI/LiveCodeBench-Pro"
              target="_blank"
              rel="noopener noreferrer"
              variant="contained"
              color="primary"
              sx={{ alignSelf: { xs: 'stretch', sm: 'flex-start' }, fontWeight: 600 }}
            >
              View on GitHub
            </Button>
          </Stack>

          <Stack spacing={3}>
            <SectionHeading title="1. Toolkit Overview" />
            <Typography variant="body1" sx={{ color: 'text.secondary' }}>
              LiveCodeBench Pro evaluates LLMs on competitive programming problems by pairing the
              LiveCodeBench-Pro dataset with a Docker-powered judge. The repository ships the
              orchestration scripts, evaluation loop, and helper utilities you need to mirror the
              official leaderboard workflow.
            </Typography>
          </Stack>

          <Stack spacing={3}>
            <SectionHeading title="2. Clone the Repository" />
            <Box component="pre" sx={codeBlockSx}>{`git clone https://github.com/GavinZhengOI/LiveCodeBench-Pro.git
cd LiveCodeBench-Pro`}</Box>
            <Typography variant="body1" sx={{ color: 'text.secondary' }}>
              All subsequent commands assume you are inside the cloned project directory.
            </Typography>
          </Stack>

          <Stack spacing={3}>
            <SectionHeading title="3. Prerequisites" />
            <Typography variant="body1" sx={{ color: 'text.secondary' }}>
              Confirm your environment matches the project requirements:
            </Typography>
            <Stack component="ul" spacing={1.5} sx={{ pl: 3, m: 0 }}>
              <ListItem>Ubuntu 20.04+ (or any distro with Linux kernel &gt;= 3.10 and cgroup support)</ListItem>
              <ListItem>Python 3.12 or newer</ListItem>
              <ListItem>pip or uv for dependency management</ListItem>
              <ListItem>Docker with permission to run containers</ListItem>
            </Stack>
          </Stack>

          <Stack spacing={3}>
            <SectionHeading title="4. Install Dependencies" />
            <Typography variant="body1" sx={{ color: 'text.secondary' }}>
              Install the Python dependencies using your preferred workflow:
            </Typography>
            <Box component="pre" sx={codeBlockSx}>{`python -m venv .venv
source .venv/bin/activate  # Windows: .venv\\Scripts\\activate
pip install -r requirements.txt`}</Box>
            <Typography variant="body1" sx={{ color: 'text.secondary' }}>
              Or sync the project with <Box component="span" sx={{ fontWeight: 600 }}>uv</Box>:
            </Typography>
            <Box component="pre" sx={codeBlockSx}>{`uv sync`}</Box>
            <Typography variant="body1" sx={{ color: 'text.secondary' }}>
              The dependency set includes evaluation utilities, dataset access helpers, and a reference
              adapter you can evolve for your own LLM provider.
            </Typography>
          </Stack>

          <Stack spacing={3}>
            <SectionHeading title="5. Prepare the Docker Runtime" />
            <Typography variant="body1" sx={{ color: 'text.secondary' }}>
              Verify Docker is installed and that your user can run containers without elevated
              privileges:
            </Typography>
            <Typography variant="body1" sx={{ color: 'text.secondary' }}>
              This setup is required for the integrated judge to compile and execute generated programs.
            </Typography>
            <Box component="pre" sx={codeBlockSx}>{`docker --version`}</Box>
            <Typography variant="body1" sx={{ color: 'text.secondary' }}>
              On Linux you may need to add your user to the <Box component="span" sx={{ fontWeight: 600 }}>docker</Box> group:
            </Typography>
            <Box component="pre" sx={codeBlockSx}>{`sudo usermod -aG docker $USER
# Log out and back in to refresh group membership`}</Box>
          </Stack>

          <Stack spacing={3}>
            <SectionHeading title="6. Implement Your LLM Interface" />
            <Typography variant="body1" sx={{ color: 'text.secondary' }}>
              Extend the abstract `LLMInterface` in `api_interface.py` and override `call_llm` to invoke
              your model. The stub below mirrors the repository template:
            </Typography>
            <Box component="pre" sx={codeBlockSx}>{`from api_interface import LLMInterface

class YourLLM(LLMInterface):
    def __init__(self):
        super().__init__()
        # Initialize clients, auth tokens, or caches here

    def call_llm(self, user_prompt: str):
        response = your_llm_client.generate(user_prompt)
        return response.text, response.metadata`}</Box>
            <Typography variant="body1" sx={{ color: 'text.secondary' }}>
              Use the built-in `ExampleLLM` for inspiration when wiring up OpenAI, self-hosted, or
              enterprise providers.
            </Typography>
          </Stack>

          <Stack spacing={3}>
            <SectionHeading title="7. Configure the Benchmark" />
            <Typography variant="body1" sx={{ color: 'text.secondary' }}>
              Point the evaluation loop at your adapter by editing `benchmark.py`. Instantiate your class
              and tune judge workers to match your machine:
            </Typography>
            <Box component="pre" sx={codeBlockSx}>{`from your_module import YourLLM

llm_instance = YourLLM()`}</Box>
            <Typography variant="body1" sx={{ color: 'text.secondary' }}>
              Keep the number of workers at or below your physical CPU cores for stable judging.
            </Typography>
          </Stack>

          <Stack spacing={3}>
            <SectionHeading title="8. Run the Benchmark" />
            <Typography variant="body1" sx={{ color: 'text.secondary' }}>
              Launch the evaluation script to iterate through the full LiveCodeBench-Pro dataset:
            </Typography>
            <Box component="pre" sx={codeBlockSx}>{`python benchmark.py`}</Box>
            <Typography variant="body1" sx={{ color: 'text.secondary' }}>
              The pipeline loads each problem, generates C++ solutions through your interface, submits
              them to the integrated judge, and records verdicts and metadata in `benchmark_result.json`.
            </Typography>
          </Stack>

          <Stack spacing={3}>
            <SectionHeading title="9. Inspect & Submit Results" />
            <Typography variant="body1" sx={{ color: 'text.secondary' }}>
              Review `benchmark_result.json` after the run. Email the artifact to
              <Box component="span" sx={{ fontWeight: 600 }}> zz4242@nyu.edu</Box> with your model details if you would like the
              results showcased on the leaderboard.
            </Typography>
          </Stack>

          <Stack spacing={3}>
            <SectionHeading title="10. Explore the Codebase" />
            <Typography variant="body1" sx={{ color: 'text.secondary' }}>
              These files are the best entry points when you extend the toolkit:
            </Typography>
            <Stack component="ul" spacing={1.5} sx={{ pl: 3, m: 0 }}>
              <ListItem>
                <Box component="span" sx={{ fontWeight: 600 }}>api_interface.py</Box> — defines the
                abstract interface and the `ExampleLLM` reference implementation.
              </ListItem>
              <ListItem>
                <Box component="span" sx={{ fontWeight: 600 }}>benchmark.py</Box> — orchestrates dataset
                loading, prompt dispatch, and result persistence.
              </ListItem>
              <ListItem>
                <Box component="span" sx={{ fontWeight: 600 }}>judge.py</Box> — integrates the
                LightCPVerifier judge and handles Docker execution.
              </ListItem>
              <ListItem>
                <Box component="span" sx={{ fontWeight: 600 }}>util.py</Box> — provides helpers such as
                `extract_longest_cpp_code` for parsing model outputs.
              </ListItem>
            </Stack>
            <Typography variant="body1" sx={{ color: 'text.secondary' }}>
              The benchmark draws from the Hugging Face datasets
              <Box component="span" sx={{ fontWeight: 600 }}> QAQAQAQAQ/LiveCodeBench-Pro</Box> and
              <Box component="span" sx={{ fontWeight: 600 }}> QAQAQAQAQ/LiveCodeBench-Pro-Testcase</Box> for comprehensive
              coverage.
            </Typography>
          </Stack>

          <Stack spacing={3}>
            <SectionHeading title="11. Stay in Touch" />
            <Typography variant="body1" sx={{ color: 'text.secondary' }}>
              Questions or bug reports? Reach out via
              <Box component="span" sx={{ fontWeight: 600 }}> zz4242@nyu.edu</Box> to coordinate on new task releases or share
              benchmarking feedback.
            </Typography>
          </Stack>
        </Stack>
      </Container>
    </Box>
  );
};

interface SectionHeadingProps {
  title: string;
}

const SectionHeading: React.FC<SectionHeadingProps> = ({ title }) => (
  <Typography variant="h5" sx={{ fontWeight: 600, fontSize: { xs: '1.25rem', md: '1.5rem' } }}>
    {title}
  </Typography>
);

const ListItem: React.FC<React.PropsWithChildren> = ({ children }) => (
  <Typography component="li" variant="body1" sx={{ color: 'text.secondary', listStyle: 'disc' }}>
    {children}
  </Typography>
);
