# CSE333-Query-Processor
A basic web search engine that takes in a query and produces page results from best match to least. It does this by keeping track of how many times the query shows up in each file and how they compare to each other. You can click on the results links and the pages will show up in the browser. 

We dealt with two security vulnerabilities in our search engine:
  The first was a "cross-site scripting flaw". This is an attack in which an attacker injects malicious executable scripts into the code of a trusted application or website.
  To fix this flaw, we needed to "escape" untrusted input from the client before you relay it to output.
  
  For the second, it was a "directory traversal attack". Instead of trusting the file pathname provided by a client, we needed to normalize the path and verify that it names a file within our document subdirectory tree (which would be ../projdocs/ if the example command shown in Part A was used to start the server). 
  If the provided path names something outside of that subdirectory, we returned an error message instead of the file contents.
