DIAG(FATAL_ERROR, OPTION_INVALID, "invalid option '%s'")
DIAG(FATAL_ERROR, OPTION_MISSING_VALUE, "missing value for option '%s'")
DIAG(FATAL_ERROR, OPTION_INVALID_VALUE, "invalid value '%s' for option '-%s'")
DIAG(FATAL_ERROR, NO_INPUT_FILE, "no input file given")
DIAG(FATAL_ERROR, MULTIPLE_INPUT_FILES, "multiple input files given")
DIAG(FATAL_ERROR, OPTION_DUPLICATE, "duplicate option '-%s'")
DIAG(FATAL_ERROR, NO_OUTPUT, "no output path provided")

DIAG_FILE(FATAL_ERROR, UNKNOWN_FILE_EXTENSION, "unknown file extension '%s'")
DIAG_FILE(FATAL_ERROR, INPUT_OUTPUT_SAME_EXTENSION, "input and output files have the same extension")
DIAG_FILE(FATAL_ERROR, CANNOT_CONVERT_FROM_8XP, "cannot convert from 8xp format")
DIAG_FILE(FATAL_ERROR, CANNOT_CONVERT_TO_TIBASICPP, "cannot convert to TI-BASIC++ source")
DIAG_FILE(FATAL_ERROR, SEND_OUTPUT_NOT_8XP, "output must be an 8xp program to send to calculator")

DIAG_FILE(FATAL_ERROR, FILE_MISSING, "no such file")
DIAG_FILE(FATAL_ERROR, PATH_IS_A_DIRECTORY, "is a directory")
DIAG_FILE(FATAL_ERROR, FILE_NOT_REGULAR, "not a regular file")

DIAG_FILE(FATAL_ERROR, FILE_OPEN_FAILED, "failed to open file")

DIAG_FILE(FATAL_ERROR, 8XP_FILENAME_TOO_LONG, "output 8xp program name cannot be longer than 8 characters")
DIAG_FILE(FATAL_ERROR, 8XP_FILENAME_EMPTY, "output 8xp program name cannot be empty")
DIAG_FILE(FATAL_ERROR, 8XP_FILENAME_NOT_UPPERCASE, "output 8xp program name must be uppercase")
DIAG_FILE(FATAL_ERROR, 8XP_FILENAME_NOT_ALNUM, "output 8xp program name must be alphanumeric")

DIAG_LOCATION(FATAL_ERROR, TOKEN_INVALID, "invalid token '%c' (0x%02x)")
DIAG_LOCATION(FATAL_ERROR, UNTERMINATED_BLOCK_COMMENT, "unterminated /* comment")
DIAG_LOCATION(FATAL_ERROR, UNTERMINATED_STRING_LITERAL, "unterminated string literal")

DIAG_LOCATION(FATAL_ERROR, INVALID_SUFFIX_ON_NUMERIC_LITERAL, "invalid suffix on numeric literal '%c'")
DIAG_LOCATION(FATAL_ERROR, INVALID_SUFFIX_ON_OCTAL_LITERAL, "invalid suffix on octal literal '%c'")
DIAG_LOCATION(FATAL_ERROR, INVALID_SUFFIX_ON_BINARY_LITERAL, "invalid suffix on binary literal '%c'")
DIAG_LOCATION(FATAL_ERROR, INVALID_SUFFIX_ON_HEXADECIMAL_LITERAL, "invalid suffix on hexadecimal literal '%c'")

DIAG_LOCATION(FATAL_ERROR, UNEXPECTED_TOKEN, "unexpected %s")
DIAG_LOCATION(FATAL_ERROR, UNEXPECTED_PUNCTUATOR, "unexpected punctuator '%s'")
DIAG_LOCATION(FATAL_ERROR, UNEXPECTED_KEYWORD, "unexpected keyword '%s'")

DIAG_LOCATION(FATAL_ERROR, UNEXPECTED_TOKEN_EXPECTED, "unexpected %s, expected %s")
DIAG_LOCATION(FATAL_ERROR, UNEXPECTED_PUNCTUATOR_EXPECTED, "unexpected punctuator '%s', expected %s")
DIAG_LOCATION(FATAL_ERROR, UNEXPECTED_KEYWORD_EXPECTED, "unexpected keyword '%s', expected %s")

DIAG_LOCATION(ERROR, INVALID_MATRIX_VARIABLE_NAME, "invalid matrix variable name, must be [A-J]")
DIAG_LOCATION(ERROR, INVALID_VARIABLE_NAME, "invalid variable name '%.*s'")

DIAG_LOCATION(ERROR, VOID_VARIABLE_DECLARATION, "variable declaration cannot have type 'void'")
DIAG_LOCATION(FATAL_ERROR, ELIF_WITHOUT_IF, "elif without matching if")
DIAG_LOCATION(FATAL_ERROR, ELSE_WITHOUT_IF, "else without matching if")
DIAG_LOCATION(ERROR, RESERVE_NOT_GLOBAL, "reserve declarations are not allowed in function bodies")

DIAG_LOCATION(WARNING, EMPTY_CONDITION_STATEMENT_BODY, "empty %s statement body")

