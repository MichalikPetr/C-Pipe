# C-Pipe
My implementation of pipes in C

## Functions
**pipe_create(unsigned int size)** - creates new pipe of designated size and returns a pointer to it   
  
**pipe_write(struct pipe \*pipe, unsigned char \*data, unsigned int size)** - writes to pipe byte by byte. Uses mutex lock to avoid collisions.
If pipe gets closed while writing, returns amount of bytes written to it before closing. When it finishes writing, returns amount of bytes written   
  
**int pipe_read(struct pipe \*pipe, unsigned char \*data, unsigned int size)** - reads data from pipe byte by byte. If pipe gets closed while reading, returns amount of bytes read to it before closing. When it finishes reading, returns amount of bytes written   
  
**pipe_close(struct pipe \*pipe)** - closes the pipe by locking it, setting closed flag to true and unlocking it   
  
**pipe_free(struct pipe \*pipe)** - frees the memory taken up by pipe
