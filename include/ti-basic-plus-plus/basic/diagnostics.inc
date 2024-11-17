DIAG(FATAL_ERROR, OPTION_INVALID, "invalid option '-%s'")
DIAG(FATAL_ERROR, OPTION_MISSING_VALUE, "missing value for option '-%s'")
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
