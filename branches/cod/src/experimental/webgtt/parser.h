// Copyright (c) 2011 The Native Client Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be found
// in the LICENSE file.

#ifndef EXPERIMENTAL_WEBGTT_PARSER_H_
#define EXPERIMENTAL_WEBGTT_PARSER_H_

/// @fileoverview This file provides a helper class with functions that parse
/// and validate the message received by the NaCl module, decode the same, and
/// obtain the appropriate response to be sent back to the browser.
///
/// @author ragad@google.com (Raga Gopalakrishnan)

#include <string>
#include <vector>

#include "webgtt/taskmap.h"

namespace graph {
class Graph;
}

namespace webgtt {

/// The integer value that represents an invalid value.
const int kInvalidValue = -1;

/// The Parser class. This class provides an interface for validating a given
/// message and parsing it to decode the message into its constituent entities.
/// In addition, it also provides a function that obtains the appropriate
/// response to be sent back to the browser.
class Parser {
 public:
  /// The constructor takes in a message, and sets default values to its class
  /// members.
  ///
  /// @param[in] message The input message to be parsed/validated.
  /// @constructor
  explicit Parser(const std::string& message);

  /// This function starts decoding the message into its constituent components
  /// (adjacency matrix, task ID, list of arguments). During this process, if
  /// the message is found to be invalid, parsing is aborted, and the is_valid_
  /// bit would contain false. Upon successful completion, the is_valid_ bit is
  /// set to true.
  ///
  /// @return false if an error was encountered, true otherwise.
  bool DecodeMessage();

  /// This function returns the response string to be sent back to the browser.
  ///
  /// This function should be used only when is_valid_ is true.
  ///
  /// @return The response string to be sent back to the browser.
  std::string GetResponse() const;

 private:
  std::string message_;
  bool is_valid_;
  std::vector< std::vector<int> > adjacency_matrix_;
  int task_ID_;
  std::vector<int> args_;

  /// The information about the function to be called, corresponding to a given
  /// task ID.
  std::vector<FunctionInfo> task_map_;
  /// This disallows usage of copy and assignment constructors.
  Parser(const Parser&);
  void operator=(const Parser&);
};

/// This helper function converts a string in CSV format into a vector of the
/// integer equivalents (using strtoi below) of its component elements.
///
/// @param[in] message The input string in CSV format.
/// @return The vector of integer equivalents of the component elements.
std::vector<int> DecodeCSV(const std::string& message);

/// This helper function returns the next chunk of the message to be processed,
/// starting from parse_position, until the sentinel is encountered. It also
/// updates the position to continue parsing from.
///
/// @param[in] message The message to take the next chunk out of.
/// @param[in,out] parse_position The starting position of the chunk.
/// @return The substring of message starting at parse_position, until the
///         sentinel is encountered. Returns the sentinel itself to indicate an
///         error if the sentinel is not encountered.
std::string GetNextChunk(const std::string& message, int* parse_position);

/// This helper function returns the positions where a comma occurs in the given
/// message.
///
/// @param[in] message The message to look for commas.
/// @return A vector of positions where a comma occurs in the message.
std::vector<int> GetCommaPositions(const std::string& message);


/// This helper function converts a string to an integer, internally using atoi.
///
/// @param[in] message The string to be converted.
/// @return kInvalidValue if the string is empty or doesn't begin with a digit.
///     Otherwise, the integer value that would be returned by atoi.
int StringToInteger(const std::string& message);

}  // namespace webgtt

#endif  // EXPERIMENTAL_WEBGTT_PARSER_H_