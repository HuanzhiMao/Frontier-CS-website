import React from 'react';

export type OverviewFilter = 'all' | 'Publication' | 'Release';

export interface OverviewFilterContextValue {
  filter: OverviewFilter;
  setFilter: (filter: OverviewFilter) => void;
}

export const OverviewFilterContext = React.createContext<OverviewFilterContextValue>({
  filter: 'all',
  setFilter: () => {},
});

export const useOverviewFilter = () => React.useContext(OverviewFilterContext);

