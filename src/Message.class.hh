<?hh // strict
  class Message {  //DUMMY! TODO!
    public function getRecipients(): Vector<int> {
      throw new Exception("unimplemented");
    }

    public function getSendableString(): string {
      throw new Exception("unimplemented");
      return "";
    }
  }
