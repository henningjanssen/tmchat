<?hh // strict

type PDOInfo = shape(
  'stmt' => string,
  'values' => Map<string, shape(
    "pdokey" => string,
    "msgkey" => string,
    "dbkey" => string,
    "type" => string
  )>
);
