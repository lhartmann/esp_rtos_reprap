#! /bin/bash
fix="false"
[ "$1" == "--fix" ] && fix="true"

find . -iname "*.h" | while read F; do
	grep -q "extern \"C\"" "$F" \
	&& echo "$F is ok." \
	|| {
		echo -n "$F is missing C++ protection"
		if $fix; then 
			echo ", adding..."
			
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
		else
			echo "."
		fi
	}
done
