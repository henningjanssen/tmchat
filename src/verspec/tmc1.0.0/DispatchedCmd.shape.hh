<?hh // strict

type DispatchedCmd = shape(
  'path' => string,
  'parameters' => Map<string, string>
);
