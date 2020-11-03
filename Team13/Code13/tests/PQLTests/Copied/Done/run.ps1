Invoke-Expression -Command "autotester.exe iter1_1_source.txt iter1_1_queries.txt xmls/iter1_1.xml"
Invoke-Expression -Command "autotester.exe iter1_2_source_IfNestedinWhile.txt iter1_2_queries_IfNestedinWhile.txt xmls/iter1_2.xml"
Invoke-Expression -Command "autotester.exe iter1_3_source_Validation.txt iter1_3_queries_Validation.txt xmls/iter1_3.xml"
Invoke-Expression -Command "autotester.exe iter1_4_source_WhileNestedinIf.txt iter1_4_queries_WhileNestedinIf.txt xmls/iter1_4.xml"

Invoke-Expression -Command "autotester.exe iter2_1_source_calls.txt iter2_1_queries_calls.txt xmls/iter2_1.xml"
Invoke-Expression -Command "autotester.exe iter2_2_source_alternate_output_attributes.txt iter2_2_queries_alternate_output_attributes.txt xmls/iter2_2.xml"
Invoke-Expression -Command "autotester.exe iter2_3_source_multiClauses.txt iter2_3_queries_multiClauses.txt xmls/iter2_3.xml"
Invoke-Expression -Command "autotester.exe iter2_4_source_complex_callst_graph.txt iter2_4_queries_complex_callst_graph.txt xmls/iter2_4.xml"
Invoke-Expression -Command "autotester.exe iter2_5_source_complex_calls_graph.txt iter2_5_queries_complex_calls_graph.txt xmls/iter2_5.xml"
Invoke-Expression -Command "autotester.exe iter2_6_source_with.txt iter2_6_queries_with.txt xmls/iter2_6.xml"
Invoke-Expression -Command "autotester.exe iter2_7_newFeatures_source.txt iter2_7_newFeatures_queries.txt xmls/iter2_7.xml"
Invoke-Expression -Command "autotester.exe iter2_8_source_NextT.txt iter2_8_queries_NextT.txt xmls/iter2_8.xml"

Invoke-Expression -Command "autotester.exe iter3_1_source_BOOLEAN.txt iter3_1_queries_BOOLEAN.txt xmls/iter3_1.xml"
Invoke-Expression -Command "autotester.exe iter3_2_source_patternAssign.txt iter3_2_queries_patternAssign.txt xmls/iter3_2.xml"

