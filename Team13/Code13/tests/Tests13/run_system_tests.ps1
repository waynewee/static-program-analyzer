Invoke-Expression -Command "autotester.exe basic_source.txt queries_no_clauses.txt xmls/basic_no_clause.xml"

Invoke-Expression -Command "autotester.exe basic_source.txt queries_one_calls.txt xmls/basic_one_calls.xml"
Invoke-Expression -Command "autotester.exe basic_source.txt queries_one_calls_s.txt xmls/basic_one_calls_s.xml"

Invoke-Expression -Command "autotester.exe basic_source.txt queries_one_follows.txt xmls/basic_one_follows.xml"
Invoke-Expression -Command "autotester.exe basic_source.txt queries_one_follows_s.txt xmls/basic_one_follows_s.xml"

Invoke-Expression -Command "autotester.exe basic_source.txt queries_one_modifies.txt xmls/basic_one_modifies.xml"
Invoke-Expression -Command "autotester.exe basic_source.txt queries_one_modifies_s.txt xmls/basic_one_modifies_s.xml"

Invoke-Expression -Command "autotester.exe basic_source.txt queries_one_next.txt xmls/basic_one_next.xml"

Invoke-Expression -Command "autotester.exe basic_source.txt queries_one_parent.txt xmls/basic_one_parent.xml"
Invoke-Expression -Command "autotester.exe basic_source.txt queries_one_parent_s.txt xmls/basic_one_parent_s.xml"

Invoke-Expression -Command "autotester.exe basic_source.txt queries_one_pattern.txt xmls/basic_one_pattern.xml"

Invoke-Expression -Command "autotester.exe basic_source.txt queries_one_uses.txt xmls/basic_one_uses.xml"
Invoke-Expression -Command "autotester.exe basic_source.txt queries_one_uses_s.txt xmls/basic_one_uses_s.xml"
