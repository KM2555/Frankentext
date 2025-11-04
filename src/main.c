#include <ctype.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_WORD_COUNT 15'000
#define MAX_SUCCESSOR_COUNT MAX_WORD_COUNT / 2

char book[] = {
#embed "pg84.txt" /// Stores the content of the file as an array of chars.
    , '\0'};      /// Makes `book` a string.

/// Array of tokens registered so far.
/// No duplicates are allowed.
char *tokens[MAX_WORD_COUNT];
/// `tokens`'s current size
size_t tokens_size = 0;

/// Array of successor tokens
/// One token can have many successor tokens. `succs[x]` corresponds to
/// `token[x]`'s successors.
/// We store directly tokens instead of token_ids, because we will directly
/// print them. If we wanted to delete the book, then it would make more sense
/// to store `token_id`s
char *succs[MAX_WORD_COUNT][MAX_SUCCESSOR_COUNT];
/// `succs`'s current size
size_t succs_sizes[MAX_WORD_COUNT];

/// Overwrites non-printable characters in `book` with a space.
/// Non-printable characters may lead to duplicates like
/// `"\xefthe" and "the"` even both print `the`.
void replace_non_printable_chars_with_space() {
  // YOUR CODE HERE
   for (char *p = book; *p != '\0'; ++p) {
        if (!isprint((unsigned char)*p)) {
            *p = ' ';
        }
    }
}
/// Returns the id (index) of the token, creating it if necessary.
///
/// Returns token id if token exists in \c tokens, otherwise creates a new entry
/// in \c tokens and returns its token id.
///
/// \param token token to look up (or insert)
/// \return Index of `token` in \c tokens array
size_t token_id(char *token) {
  size_t id;
  for (id = 0; id < tokens_size; ++id) {
    if (strcmp(tokens[id], token) == 0) {
      return id;
    }
  }
  tokens[id] = token;
  ++tokens_size;
  return id;
}

/// Appends the token \c succ to the successors list of \c token.
void append_to_succs(char *token, char *succ) {
  auto next_empty_index_ptr = &succs_sizes[token_id(token)];

  if (*next_empty_index_ptr >= MAX_SUCCESSOR_COUNT) {
    printf("Successor array full.");
    exit(EXIT_FAILURE);
  }

  succs[token_id(token)][(*next_empty_index_ptr)++] = succ;
}

/// Creates tokens on \c book and fills \c tokens and \c succs using
/// the functions \c token_id and \c append_to_succs.
void tokenize_and_fill_succs(char *delimiters, char *str) {
  // YOUR CODE HERE
  char *token_str = strtok(str, delimiters);
    char *prev_token = NULL;

    while (token_str != NULL) {
  // Ensure token is registered (id is not needed here)
  token_id(token_str);

        // If previous token exists, record successor
        if (prev_token != NULL) {
            append_to_succs(prev_token, token_str);
        }

        prev_token = token_str;
        token_str = strtok(NULL, delimiters);
    }
}


/// Returns last character of a string
char last_char(char *str) {
  // YOUR CODE HERE
  size_t len = strlen(str);
    if (len == 0) return '\0';
    return str[len - 1];
}

/// Returns whether the token ends with `!`, `?` or `.`.
bool token_ends_a_sentence(char *token) {
  // YOUR CODE HERE
   size_t len = strlen(token);
    if (len == 0) return false;
    char c = token[len - 1];
    return (c == '.' || c == '!' || c == '?');
}

/// Returns a random `token_id` that corresponds to a `token` that starts with a
/// capital letter.
/// Uses \c tokens and \c tokens_size.
size_t random_token_id_that_starts_a_sentence() {
  // YOUR CODE HERE
  size_t indices[MAX_WORD_COUNT];
    size_t count = 0;

    for (size_t i = 0; i < tokens_size; ++i) {
        if (isupper((unsigned char)tokens[i][0])) {
            if (count >= MAX_WORD_COUNT) break; // precaution
            indices[count++] = i;
        }
    }
    if (count == 0) {
        // fallback: pick any token randomly
        return rand() % tokens_size; 
    }
    return indices[rand() % count];
}

/// Generates a random sentence using \c tokens, \c succs, and \c succs_sizes.
/// The sentence array will be filled up to \c sentence_size-1 characters using
/// random tokens until:
/// - a token is found where \c token_ends_a_sentence
/// - or more tokens cannot be concatenated to the \c sentence anymore.
/// Returns the filled sentence array.
///
/// @param sentence array what will be used for the sentence.
//
//                  Will be overwritten. Does not have to be initialized.
/// @param sentence_size
/// @return input sentence pointer
char *generate_sentence(char *sentence, size_t sentence_size) {
  size_t current_token_id = random_token_id_that_starts_a_sentence();
  auto token = tokens[current_token_id];

  sentence[0] = '\0';
  strcat(sentence, token);
  if (token_ends_a_sentence(token))
    return sentence;

  // Calculated sentence length for the next iteration.
  // Used to stop the loop if the length exceeds sentence size
  size_t sentence_len_next;
  // Concatenates random successors to the sentence as long as
  // `sentence` can hold them.
  do {
    // YOUR CODE HERE
      size_t current_token_index = current_token_id;
        size_t succ_count = succs_sizes[current_token_index];

        if (succ_count == 0)
            break; // no successors, stop generating

        // Pick a random successor token ID
        size_t next_token_id = rand() % succ_count;
        char *next_token = succs[current_token_index][next_token_id];

        size_t current_len = strlen(sentence);
        size_t next_token_len = strlen(next_token);

        // Check if adding space + next token exceeds buffer size
        if (current_len + 1 + next_token_len >= sentence_size)
            break;

        // Append space and token
        strcat(sentence, " ");
        strcat(sentence, next_token);
        current_token_id = token_id(next_token);

        // End if token ends a sentence
        if (token_ends_a_sentence(next_token))
            break;

        sentence_len_next = strlen(sentence);
  } while (sentence_len_next < sentence_size - 1);
  return sentence;
}

int main() {
  replace_non_printable_chars_with_space();

  char *delimiters = " \n\r";
  tokenize_and_fill_succs(delimiters, book);

  char sentence[1000];
  srand(time(NULL)); // Be random each time we run the program

  // Generate sentences until we find a question sentence.
  do {
    generate_sentence(sentence, sizeof sentence);
  } while (last_char(sentence) != '?');
  puts(sentence);
  puts("");

  // Initialize `sentence` and then generate sentences until we find a sentence
  // ending with an exclamation mark.
  do {
    generate_sentence(sentence, sizeof sentence);
  } while (last_char(sentence) != '!');
  puts(sentence);
}