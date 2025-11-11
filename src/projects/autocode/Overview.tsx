import React from 'react';
import { Box, Card, CardContent, Stack, Typography } from '@mui/material';
import frameworkImg from '../../assets/autocode/framework.png';
import problemLevelImg from '../../assets/autocode/problem_level.png';

export const AutoCodeOverview: React.FC = () => (
  <Stack spacing={{ xs: 5, md: 6 }} sx={{ width: '100%', maxWidth: 960, mx: 'auto' }}>
    <Stack spacing={{ xs: 2.5, md: 3 }} sx={{ width: '100%' }}>
      <Box
        sx={{
          border: '1px solid',
          borderColor: 'divider',
          borderRadius: 2,
          p: { xs: 2, md: 2.5 },
          bgcolor: 'background.paper',
        }}
      >
        <Stack spacing={0.5}>
          <Typography variant="body1" sx={{ color: 'text.secondary', fontStyle: 'italic' }}>
            An AI system can create and maintain knowledge only to the extent that it can verify that knowledge itself.
          </Typography>
          <Typography variant="body1" sx={{ color: 'text.secondary', fontStyle: 'italic' }}>
            — Rich Sutton, Verification, The Key to AI, 2001
          </Typography>
        </Stack>
      </Box>
      <Typography variant="body1" sx={{ color: 'text.secondary' }}>
        Posing the problem is often more critical than solving it. AutoCode is a framework that lets LLMs create and verify competitive programming problems, mirroring the workflow of human problem setters.
      </Typography>
      <Typography variant="body1" sx={{ color: 'text.secondary' }}>
        Through seed problems and a double-verification protocol, AutoCode can autonomously generate new problems while guaranteeing correctness. By having the LLM play the roles of generator, validator, and checker, the system produces trustworthy test data.
      </Typography>
      <Typography variant="body1" sx={{ color: 'text.secondary' }}>
        Human experts confirm that AutoCode can deliver high-quality problems ready for ICPC/IOI.
      </Typography>
    </Stack>

    <Stack spacing={{ xs: 3, md: 4 }}>
      <Stack spacing={{ xs: 2, md: 2.5 }}>
        <Card elevation={0} sx={{ bgcolor: 'rgba(79, 70, 229, 0.08)', borderRadius: 3 }}>
          <CardContent sx={{ px: { xs: 3, md: 4 }, py: { xs: 3, md: 4 } }}>
            <Stack spacing={2.5} sx={{ mb: 2.5 }}>
              <Stack spacing={1}>
                <Typography variant="body1" sx={{ fontWeight: 600, color: 'text.primary', mb: 0.5 }}>
                  Combined benchmark
                </Typography>
                <Typography variant="caption" sx={{ color: 'text.secondary', letterSpacing: '0.02em', mt: -0.5 }}>
                  7,538 problems · 26 human submissions per problem · 50% pass rate
                </Typography>
                <Box
                  sx={{
                    display: 'grid',
                    gridTemplateColumns: { xs: 'repeat(2, minmax(0, 1fr))', sm: 'repeat(3, minmax(0, 1fr))' },
                    gap: { xs: 1.5, md: 2 },
                  }}
                >
                  <Box>
                    <Typography variant="caption" sx={{ color: 'text.secondary' }}>
                      Consistency (↑)
                    </Typography>
                    <Typography variant="body1" sx={{ fontWeight: 600 }}>
                      91.1%
                    </Typography>
                  </Box>
                  <Box>
                    <Typography variant="caption" sx={{ color: 'text.secondary' }}>
                      False Positive Rate (↓)
                    </Typography>
                    <Typography variant="body1" sx={{ fontWeight: 600 }}>
                      3.7%
                    </Typography>
                  </Box>
                  <Box>
                    <Typography variant="caption" sx={{ color: 'text.secondary' }}>
                      False Negative Rate (↓)
                    </Typography>
                    <Typography variant="body1" sx={{ fontWeight: 600 }}>
                      14.1%
                    </Typography>
                  </Box>
                </Box>
              </Stack>

              <Stack spacing={1}>
                <Typography variant="body1" sx={{ fontWeight: 600, color: 'text.primary' }} >
                  Recent Codeforces set
                </Typography>
                <Typography variant="caption" sx={{ color: 'text.secondary', letterSpacing: '0.02em', mt: -0.5 }}>
                  720 problems · includes interactives · 33 LLM submissions per problem · 25% pass rate
                </Typography>
                <Box
                  sx={{
                    display: 'grid',
                    gridTemplateColumns: { xs: 'repeat(2, minmax(0, 1fr))', sm: 'repeat(3, minmax(0, 1fr))' },
                    gap: { xs: 1.5, md: 2 },
                  }}
                >
                  <Box>
                    <Typography variant="caption" sx={{ color: 'text.secondary' }}>
                      Consistency (↑)
                    </Typography>
                    <Typography variant="body1" sx={{ fontWeight: 600 }}>
                      98.7%
                    </Typography>
                  </Box>
                  <Box>
                    <Typography variant="caption" sx={{ color: 'text.secondary' }}>
                      False Positive Rate (↓)
                    </Typography>
                    <Typography variant="body1" sx={{ fontWeight: 600 }}>
                      1.3%
                    </Typography>
                  </Box>
                  <Box>
                    <Typography variant="caption" sx={{ color: 'text.secondary' }}>
                      False Negative Rate (↓)
                    </Typography>
                    <Typography variant="body1" sx={{ fontWeight: 600 }}>
                      1.2%
                    </Typography>
                  </Box>
                </Box>
              </Stack>
            </Stack>
            <Typography variant="body2" sx={{ color: 'text.secondary' }}>
              AutoCode's generated test suites outperform current SOTA tools such as CodeContests, CodeContests+, TACO, and HardTests, whose consistency sits between 72% and 81%.
            </Typography>
          </CardContent>
        </Card>

        <Stack spacing={{ xs: 1.5, md: 2 }}>
          <Typography variant="subtitle1" sx={{ fontWeight: 600 }}>
            Problem Generation
          </Typography>
          <Box component="figure" sx={{ m: 0, borderRadius: 3, overflow: 'hidden' }}>
            <Box
              component="img"
              src={problemLevelImg}
              alt="Evaluation funnel for AutoCode-generated problems"
              sx={{ width: '100%', maxHeight: 420, objectFit: 'cover' }}
            />
            <Typography variant="caption" sx={{ mt: 1.5, display: 'block', color: 'text.secondary' }}>
              Grandmaster-level reviewers assessing the quality tiers of AutoCode problems.
            </Typography>
          </Box>
          <Typography variant="body2" sx={{ color: 'text.secondary' }}>
            The double-verification protocol raises the accuracy of LLM reference solutions from 86% to 94%. Grandmaster-level reviewers report that 61.6% of generated problems are ready for model training, 27.4% suit human instruction, 17.4% are considered novel, and 3.2% meet ICPC/IOI caliber.
          </Typography>
        </Stack>
      </Stack>

      <Stack spacing={{ xs: 2.5, md: 3 }}>
        <Typography variant="h6" sx={{ fontWeight: 600 }}>
          How AutoCode Works
        </Typography>
        <Box component="figure" sx={{ m: 0, borderRadius: 3, overflow: 'hidden' }}>
          <Box
            component="img"
            src={frameworkImg}
            alt="AutoCode generator-validator-checker workflow"
            sx={{ width: '100%', maxHeight: 520, objectFit: 'cover' }}
          />
          <Typography variant="caption" sx={{ mt: 1.5, display: 'block', color: 'text.secondary' }}>
            AutoCode generates and verifies problems through a generator–validator–checker framework.
          </Typography>
        </Box>
        <Stack spacing={{ xs: 2, md: 2.5 }}>
          <Box>
            <Typography variant="subtitle1" sx={{ fontWeight: 600, mb: 1 }}>
              Test Generation
            </Typography>
            <Stack spacing={1.25} sx={{ color: 'text.secondary' }}>
              <Typography variant="body1" sx={{ color: 'text.secondary' }}>
                1. Validator: ensures each input obeys the statement's constraints. We ask the LLM to draft 40 boundary-valid and invalid cases, propose three validator candidates, and keep the most accurate one dramatically lowering the false negative rate.
              </Typography>
              <Typography variant="body1" sx={{ color: 'text.secondary' }}>
                2. Generator: produces exhaustive small cases, randomized stress tests, and inputs designed to induce TLE. Validator filtering removes illegal samples, yielding diverse yet compliant datasets that cut the false positive rate.
              </Typography>
              <Typography variant="body1" sx={{ color: 'text.secondary' }}>
                3. Checker: compares candidate outputs against references to guarantee verdict correctness. The LLM fabricates 40 tuples of inputs, answers, borderline outputs, and expected judgments, then proposes three checker implementations; we retain the most reliable, supporting multi-solution problems.
              </Typography>
            </Stack>
          </Box>
          <Box>
            <Typography variant="subtitle1" sx={{ fontWeight: 600, mb: 1 }}>
              Problem Generation
            </Typography>
            <Stack spacing={1.25} sx={{ color: 'text.secondary' }}>
              <Typography variant="body1" sx={{ color: 'text.secondary' }}>
                1. Seed problems: start from a randomly selected seed and guide the LLM to add, remove, or refine conditions to create new statements.
              </Typography>
              <Typography variant="body1" sx={{ color: 'text.secondary' }}>
                2. Double-verification protocol: the LLM must supply both a reference solution and a simple but slow brute-force answer. Only when both agree on every test case is the new problem accepted.
              </Typography>
            </Stack>
          </Box>
        </Stack>
      </Stack>

      <Stack spacing={{ xs: 2, md: 2.5 }}>
        <Typography variant="h6" sx={{ fontWeight: 600 }}>
          What We Learned About LLM Creativity
        </Typography>
        <Stack spacing={1.25} sx={{ color: 'text.secondary' }}>
          <Typography variant="body1" sx={{ color: 'text.secondary' }}>
            1. LLMs can author problems they cannot solve: the models produce challenging, solvable problems that still stump themselves, a useful reservoir for future self-improvement.
          </Typography>
          <Typography variant="body1" sx={{ color: 'text.secondary' }}>
            2. New problems often remix familiar patterns: LLMs prefer combining existing algorithms and tricks over inventing brand-new paradigms.
          </Typography>
          <Typography variant="body1" sx={{ color: 'text.secondary' }}>
            3. LLMs struggle to judge problem quality: expert assessments correlate weakly with the model's own judgments. Difficulty is a more reliable proxy for usefulness than the model's intrinsic score.
          </Typography>
        </Stack>
      </Stack>

      <Stack spacing={{ xs: 1.5, md: 2 }}>
        <Typography variant="h6" sx={{ fontWeight: 600 }}>
          Closing Thoughts
        </Typography>
        <Typography variant="body1" sx={{ color: 'text.secondary', maxWidth: 720 }}>
          AutoCode pushes LLM capabilities from solving problems to inventing and validating them. It characterises the creative potential of modern models while delivering a practical framework for dependable test data. We look forward to seeing AutoCode applied across contests, education, research, engineering, and self-play.
        </Typography>
      </Stack>
    </Stack>
  </Stack>
);
