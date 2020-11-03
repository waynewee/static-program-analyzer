Invoke-Expression -Command "autotester.exe iter1_1_source.txt iter1_1_queries.txt xmls/iter1_1.xml"
Invoke-Expression -Command "autotester.exe iter1_2_source_IfNestedinWhile.txt iter1_2_queries_IfNestedinWhile.txt xmls/iter1_2.xml"
Invoke-Expression -Command "autotester.exe iter1_3_source_Validation.txt iter1_3_queries_Validation.txt xmls/iter1_3.xml"
Invoke-Expression -Command "autotester.exe iter1_4_source_WhileNestedinIf.txt iter1_4_queries_WhileNestedinIf.txt xmls/iter1_4.xml"
