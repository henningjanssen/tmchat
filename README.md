# Short story

This project was meant to be a chat server for an old app-project including a chat-function. As this project was abandoned this chat-server has been finished and probably never will be. It was supposed to support multiple protocols but only one custom irc-like protocol has been half implemented. This server was meant to serve a small user-count. It is entirely written in Hack.

#TM Chat-Server

##Documentation
Any documentation is provided as _tex_-files.
To build _pdf_-document you can use `make doc` to compile all documentation-_tex_-files into _pdf_ s using _pdflatex_ and remove all further created files on the fly.

* `make confdoc` for _configuration_-doc (regarding the configuration-file)
* `make protocoldoc` for _protocol_-doc (connection and communication between server and client)
* `make cleandoc` removes all files (incl. _pdf_) that are created by _pdflatex_ during compilation
