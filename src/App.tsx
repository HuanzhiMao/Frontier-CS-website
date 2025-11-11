import './App.css'
import { Leaderboard, RatingTrendPage, geekTheme } from './leaderboard'
import { BrowserRouter as Router, Navigate, Route, Routes } from 'react-router-dom'
import { Layout } from './layout'
import { ThemeProvider, CssBaseline } from '@mui/material';
import { ProjectsOverview } from './overview'
import { LivecodeOverview } from './projects/livecodebench-pro/Overview'
import { AutoCodeOverview } from './projects/autocode/Overview'
import { LivecodeLocalEvaluationBlog } from './blog'
import { SubmitStandardJudge } from './projects/livecodebench-pro/SubmitStandardJudge'
import { TaskStatus } from './projects/livecodebench-pro/TaskStatus'

// Removed ThemeProvider, CssBaseline, Router, Routes, Layout, Problems, About, and related imports
// Also removing FaGithub, FaTrophy, useColorScheme, scrollToId for now to simplify

function App() {
  // const { mode } = useColorScheme() // Not needed for just leaderboard
  // if (!mode) return null;

  // const scrollToId = useCallback((id: string) => { // Not needed for just leaderboard
  //   const el = document.getElementById(id);
  //   if (el) {
  //     el.scrollIntoView({ behavior: 'smooth' });
  //   }
  // }, []);

  return (
    <ThemeProvider theme={geekTheme}>
      <CssBaseline />
      <Router>
        <Routes>
          <Route path="/" element={<Layout />}>
            <Route index element={<ProjectsOverview />} />
            <Route path="projects" element={<ProjectsOverview />} />

            <Route
              path="projects/livecodebench-pro"
              element={<Navigate to="projects/livecodebench-pro/overview" replace />}
            />
            <Route
              path="projects/livecodebench-pro/overview"
              element={<LivecodeOverview />}
            />
            <Route
              path="projects/livecodebench-pro/leaderboard"
              element={<Leaderboard />}
            />
            <Route
              path="projects/livecodebench-pro/rating-trend"
              element={<RatingTrendPage />}
            />
            <Route
              path="projects/livecodebench-pro/submit"
              element={<SubmitStandardJudge />}
            />
            <Route
              path="projects/livecodebench-pro/task/:taskId"
              element={<TaskStatus />}
            />
            <Route path="projects/autocode" element={<Navigate to="projects/autocode/overview" replace />} />
            <Route path="projects/autocode/overview" element={<AutoCodeOverview />} />
            <Route path="overview" element={<Navigate to="projects/livecodebench-pro/overview" replace />} />
            <Route path="leaderboard" element={<Navigate to="projects/livecodebench-pro/leaderboard" replace />} />
            <Route path="rating-trend" element={<Navigate to="projects/livecodebench-pro/rating-trend" replace />} />
            <Route
              path="release/livecodebench-pro-local-evaluation"
              element={<LivecodeLocalEvaluationBlog />}
            />
          </Route>
        </Routes>
      </Router>
    </ThemeProvider>
  );
}

export default App
