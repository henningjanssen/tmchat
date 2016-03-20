<?hh // strict
  require_once __DIR__.'DispatchedCmd.shape.hh';
  require_once __DIR__.'/../../exceptions/SyntaxErrorException.class.hh';

  //https://3v4l.org/p82XB

  type Param = shape(
    'key' => string,
    'value' => string
  );
  class Parser{
    private int $pos = 0;
    private string $curChar = '';
    private int $paramCounter = 0;
    public function __construct(private string $raw){
      $this->curChar = $this->raw[$this->pos];
    }

    public function parse(): DispatchedCmd {
      $path = $this->path();
      $this->skipWhitespaces();
      $params = Map{};
      $paramCounter = 0;

      while(!$this->reachedEnd()){
        $param = $this->param();
        $params[$param['key']] = $param['value'];
        if(!$this->reachedEnd() && $this->curChar != ' '){
          $this->error("Expected ' ', got '$this->curChar' at pos $this->pos");
        }
        $this->skipWhitespaces();
      }
      return shape(
        'path' => $path,
        'parameters' => $params
      );
    }

    private function param(): Param {
      $word = '';
      $key = '';
      $keyed = false;
      $parenl = 0;
      $hasParen = false;
      while(!$this->reachedEnd()){
        if($this->curChar === '('){
          $parenl++;
          if($parenl === 1){
            $this->nextChar();
            continue;
          }
        }
        elseif($this->curChar === ')'){
          $parenl--;
          if($parenl === 0){
            $this->nextChar();
            break;
          }
        }
        elseif($this->curChar === ':'){
          if($parenl === 0 && !$keyed){
            $key = $word;
            $keyed = true;
            $word = '';
            $this->nextChar();
            continue;
          }
          elseif($parenl === 0 && $keyed){
            $this->error("Param: found unexpected ':' at pos $this->pos");
          }
        }
        elseif($this->curChar === ' '){
          if($parenl === 0){
            break;
          }
        }

        $word .= $this->curChar;
        $this->nextChar();
      }
      if($key === ''){
        $key = strval($this->paramCounter++);
      }
      return shape('key'=>$key, 'value'=>$word);
    }

    private function path(): string {
      $path = "";
      if($this->curChar !== '/'){
        $this->error(
          "Invalid path: expected '/', got '$this->curChar' at pos $this->pos"
        );
      }
      do{
        $path .= $this->curChar;
        $this->nextChar();
      } while ($this->curChar !== ' ' && !$this->reachedEnd());
      return $path;
    }

    private function reachedEnd(): bool{
      return $this->curChar === '';
    }

    private function nextChar(): void{
      $this->pos++;
      if($this->pos >= strlen($this->raw)){
        $this->curChar = '';
      }
      else{
        $this->curChar = $this->raw[$this->pos];
      }
    }

    private function skipWhitespaces(): void{
      while($this->curChar === ' '){
        $this->nextChar();
      }
    }

    private function error(string $msg = ""): void {
      throw new SyntaxErrorException($msg);
    }
  }
