#! /bin/bash
find . -iname "*.h" | while read F; do
	grep -q "extern \"C\"" "$F" || {
		echo "$F is missing C++ protection, adding..."
		
		DATA="$(
			echo "#ifdef __cplusplus"
			echo "extern \"C\" {"
			echo "#endif"
			
			cat "$F"

			echo ""
			echo "#ifdef __cplusplus"
			echo "}"
			echo "#endif"
		)"
		
		echo "$DATA" > "$F"
	}
done
