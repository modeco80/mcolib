# Adapter streams

In the mcolib2 I/O library, it's desired for a stream to be "adaptable". In this case, the design
of the library makes this possible with an "adapter stream" - itself a stream, which does the adaptation.

In mcolib2, currently GzipInStream is an example source of an adapter stream (which decompresses Gzip or Zlib data).