#ifndef UTIL_FILE_PIECE__
#define UTIL_FILE_PIECE__

#include "libKenLM/util/ersatz_progress.hh"
#include "libKenLM/util/exception.hh"
#include "libKenLM/util/file.hh"
#include "libKenLM/util/have.hh"
#include "libKenLM/util/mmap.hh"
#include "libKenLM/util/string_piece.hh"

#include <cstddef>
#include <string>

#include <stdint.h>

#ifdef HAVE_ZLIB
#include <zlib.h>
#endif

namespace util {

class ParseNumberException : public Exception {
  public:
    explicit ParseNumberException(StringPiece value) throw();
    ~ParseNumberException() throw() {}
};

class GZException : public Exception {
  public:
#ifdef HAVE_ZLIB
    explicit GZException(gzFile file);
#endif
    GZException() throw() {}
    ~GZException() throw() {}
};

extern const bool kSpaces[256];

// Memory backing the returned StringPiece may vanish on the next call.  
class FilePiece {
  public:
    // 32 MB default.
    explicit FilePiece(const char *file, std::ostream *show_progress = NULL, std::size_t min_buffer = 33554432);
    // Takes ownership of fd.  name is used for messages.  
    explicit FilePiece(int fd, const char *name, std::ostream *show_progress = NULL, std::size_t min_buffer = 33554432);

    ~FilePiece();
     
    char get() { 
      if (position_ == position_end_) {
        Shift();
        if (at_end_) throw EndOfFileException();
      }
      return *(position_++);
    }

    // Leaves the delimiter, if any, to be returned by get().  Delimiters defined by isspace().  
    StringPiece ReadDelimited(const bool *delim = kSpaces) {
      SkipSpaces(delim);
      return Consume(FindDelimiterOrEOF(delim));
    }

    // Unlike ReadDelimited, this includes leading spaces and consumes the delimiter.
    // It is similar to getline in that way.  
    StringPiece ReadLine(char delim = '\n');

    float ReadFloat();
    double ReadDouble();
    long int ReadLong();
    unsigned long int ReadULong();

    // Skip spaces defined by isspace.  
    void SkipSpaces(const bool *delim = kSpaces) {
      for (; ; ++position_) {
        if (position_ == position_end_) Shift();
        if (!delim[static_cast<unsigned char>(*position_)]) return;
      }
    }

    uint64_t Offset() const {
      return position_ - data_.begin() + mapped_offset_;
    }

    const std::string &FileName() const { return file_name_; }
    
  private:
    void Initialize(const char *name, std::ostream *show_progress, std::size_t min_buffer);

    template <class T> T ReadNumber();

    StringPiece Consume(const char *to) {
      StringPiece ret(position_, to - position_);
      position_ = to;
      return ret;
    }

    const char *FindDelimiterOrEOF(const bool *delim = kSpaces);

    void Shift();
    // Backends to Shift().
    void MMapShift(uint64_t desired_begin);

    void TransitionToRead();
    void ReadShift();

    const char *position_, *last_space_, *position_end_;

    scoped_fd file_;
    const uint64_t total_size_;
    const uint64_t page_;

    std::size_t default_map_size_;
    uint64_t mapped_offset_;

    // Order matters: file_ should always be destroyed after this.
    scoped_memory data_;

    bool at_end_;
    bool fallback_to_read_;

    ErsatzProgress progress_;

    std::string file_name_;

#ifdef HAVE_ZLIB
    gzFile gz_file_;
#endif // HAVE_ZLIB
};

} // namespace util

#endif // UTIL_FILE_PIECE__
