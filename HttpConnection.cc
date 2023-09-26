/*
 * Copyright ©2023 Justin Hsia.  All rights reserved.  Permission is
 * hereby granted to students registered for University of Washington
 * CSE 333 for use solely during Winter Quarter 2023 for purposes of
 * the course.  No other use, copying, distribution, or modification
 * is permitted without prior written consent. Copyrights for
 * third-party components of this work must be honored.  Instructors
 * interested in reusing these course materials should contact the
 * author.
 */

#include <stdint.h>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <map>
#include <string>
#include <vector>

#include "./HttpRequest.h"
#include "./HttpUtils.h"
#include "./HttpConnection.h"

using std::map;
using std::string;
using std::vector;
#define BUFSIZE 1024

namespace hw4 {

static const char* kHeaderEnd = "\r\n\r\n";
static const int kHeaderEndLen = 4;

bool HttpConnection::GetNextRequest(HttpRequest* const request) {
  // Use WrappedRead from HttpUtils.cc to read bytes from the files into
  // private buffer_ variable. Keep reading until:
  // 1. The connection drops
  // 2. You see a "\r\n\r\n" indicating the end of the request header.
  //
  // Hint: Try and read in a large amount of bytes each time you call
  // WrappedRead.
  //
  // After reading complete request header, use ParseRequest() to parse into
  // an HttpRequest and save to the output parameter request.
  //
  // Important note: Clients may send back-to-back requests on the same socket.
  // This means WrappedRead may also end up reading more than one request.
  // Make sure to save anything you read after "\r\n\r\n" in buffer_ for the
  // next time the caller invokes GetNextRequest()!

  // STEP 1:
  size_t end = buffer_.find(kHeaderEnd);

  if (end == string::npos) {
    unsigned char buf[BUFSIZE];
    while (1) {
      int res = WrappedRead(fd_, buf, BUFSIZE);
      if (res == 0) {
        // EOF
        break;
      } else if (res < 0) {
        // invalid/failed read
        return false;
      } else {
        // append bytes to the end of the buffer_
        buffer_ += string(reinterpret_cast<char*>(buf), res);
        // if we find kHeaderEnd make sure to stop
        end = buffer_.find(kHeaderEnd);
        if (end != string::npos) {
          break;
        }
        continue;
      }
    }
  }

  if (end == string::npos) {
    return false;
  }

  *request = ParseRequest(buffer_.substr(0, end));
  if (request->uri() == "BAD_") {
    return false;
  }

  buffer_ = buffer_.substr(end + kHeaderEndLen);

  return true;
}

bool HttpConnection::WriteResponse(const HttpResponse& response) const {
  string str = response.GenerateResponseString();
  int res = WrappedWrite(fd_,
                         reinterpret_cast<const unsigned char*>(str.c_str()),
                         str.length());
  if (res != static_cast<int>(str.length()))
    return false;
  return true;
}

HttpRequest HttpConnection::ParseRequest(const string& request) const {
  HttpRequest req("/");  // by default, get "/".

  // Plan for STEP 2:
  // 1. Split the request into different lines (split on "\r\n").
  // 2. Extract the URI from the first line and store it in req.URI.
  // 3. For the rest of the lines in the request, track the header name and
  //    value and store them in req.headers_ (e.g. HttpRequest::AddHeader).
  //
  // Hint: Take a look at HttpRequest.h for details about the HTTP header
  // format that you need to parse.
  //
  // You'll probably want to look up boost functions for:
  // - Splitting a string into lines on a "\r\n" delimiter
  // - Trimming whitespace from the end of a string
  // - Converting a string to lowercase.
  //
  // Note: If a header is malformed, skip that line.

  // STEP 2:

  // obtain header
  string s = buffer_.substr(0, request.length());
  // split request into different lines
  vector<string> lines;
  boost::algorithm::split(lines, s,
        boost::is_any_of("\r\n"), boost::token_compress_on);

  // trim whitespace
  for (size_t i = 0; i < lines.size(); i++) {
    boost::algorithm::trim(lines[i]);
  }

  // Extract the URI from the first line and store it in req.URI
  vector<string> components;
  string first_line = lines[0];
  boost::split(components, first_line, boost::is_any_of(" "),
        boost::token_compress_on);

  // malformed header
  if (components.size() != 3) {
    req.set_uri("Not Found");
    return req;
  }
  req.set_uri(components[1]);
  // Go through the lines and store the header name and
  // value in req.headers_
  for (size_t j = 1; j < lines.size(); j++) {
    vector<string> header;
    string line = lines[j];
    boost::split(header, line, boost::is_any_of(": "),
          boost::token_compress_on);

    string header_name = header[0];
    boost::algorithm::to_lower(header_name);
    header[0] = header_name;

    if (header.size() == 2) {
      req.AddHeader(header[0], header[1]);
    }
  }

  return req;
}

}  // namespace hw4
