import re

def generate_docs(header_path, output_path):
    with open(header_path, 'r') as f:
        content = f.read()

    # Matches: /** (doc text) */ [type] [variable_name][optional_array_brackets];
    pattern = re.compile(r'/\*\*(.*?)\*/\s*(?:[a-zA-Z0-9_:<>]+[*\s]+)+([a-zA-Z0-9_]+)(?:\[.*?\])?;', re.DOTALL)
    
    matches = pattern.findall(content)
    
    with open(output_path, 'w') as out:
        out.write("// AUTO-GENERATED FILE. DO NOT EDIT.\n")
        out.write("#ifndef AUTO_DOCS_H\n#define AUTO_DOCS_H\n\n#pragma once\n\n")
        
        for doc_text, var_name in matches:
            clean_lines = []
            
            # 1. Process line by line
            for line in doc_text.split('\n'):
                # 2. Regex specifically targets leading whitespace and the first optional asterisk
                # ^\s* : Matches start of line + any spaces/tabs
                # \*?    : Matches zero or one asterisk
                # \s* : Matches any spaces immediately after the asterisk
                cleaned = re.sub(r'^\s*\*?\s*', '', line)
                
                if cleaned:
                    clean_lines.append(cleaned)
            
            # 3. Join with a single space
            clean_text = " ".join(clean_lines)
            
            # 4. Escape double quotes to prevent breaking the C++ string compilation
            clean_text = clean_text.replace('"', '\\"')
            
            out.write(f'constexpr const char* DOC_{var_name} = "{clean_text}";\n')
        
        out.write("#endif")
if __name__ == "__main__":
    generate_docs("definitions.hpp", "auto_docs.hpp")