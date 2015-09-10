<?hh // strict
  require_once '../Message.class.hh';
  
  type ParseMessageResult = shape(
    'answer' => string,
    'message' => Message
  );
