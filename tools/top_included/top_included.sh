#!/bin/sh

grep -rhoP '^\s*#\s*include\s*[<"]\K[^>"]+' \
  --include='*.c' --include='*.cpp' --include='*.cc' --include='*.cxx' \
  --include='*.h' --include='*.hpp' --include='*.hh' \
  src/ \
  | sort | uniq -c | sort -rn | head -30
