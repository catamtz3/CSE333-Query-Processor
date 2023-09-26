# Bug 1 Segmentation Fault

## A) How is your program acting differently than you expect it to?
- Our program is giving us a Segfault when we run ServerSocket.cc when it shouldn't be doing this.

## B) Brainstorm a few possible causes of the bug
- I did a backtrace in gdb and it gave me line 71 where I called getaddrinfo(). I think I incorrectly did the cast of the port number to string type.
- Maybe I incorrectly passed in one of the other parameters in getaddrinfo().
- Maybe I called the function wrong?

## C) How you fixed the bug and why the fix was necessary
- I experimented with the casting of the port number to string since it was what I was most confident was incorrect. I initially casted char* onto port_, but it was giving a warning that the cast was off due to difference in sizes. It still ran however, but was giving me a segfault. I then fixed the casting to store the port_ into a std::string variable with the port_ turned to a string and then passed that variable in, and it got rid of the segfault and was passing all of the tests! The fix was necessary because the cast and pointer were of different sizes so it wasn't passing in the correct data. I needed to fix the cast or the program would never run correctly.


# Bug 2

## A) How is your program acting differently than you expect it to?
- When we run HTTPConnection.cc, our program is overriding the headers of the previous requests if they have the same name when they shouldn't be overriding at all.

## B) Brainstorm a few possible causes of the bug
- Maybe we're processing our requests wrong and splitting them up inaccurately.
- Maybe we didn't add the header properly in ParseRequest.
- We may have passed the wrong thing as the parameter in ParseRequest.

## C) How you fixed the bug and why the fix was necessary
- We went through our functions and couldn't fault our logic, but we were unsure of if what we were passing into ParseRequest was correct. We were initially passing in the whole buffer, which explains why the requests were overriding each other. We knew we needed to split up what we were passing into ParseRequest, so we attempted to use the buffer_substr() method to only pass in one request at a time. This fixed our error and our program was running great after this! We needed the change because if we left the buffer as the parameter, it would've kept overriding the previous requests.


# Bug 3

## A) How is your program acting differently than you expect it to?
- When we run HttpUtils.cc, our program fails the IsPathSafe test. All asserts up until the last assert on line 99 of test_httputils.cc work as expected.

## B) Brainstorm a few possible causes of the bug
- We are ignoring all characters that are not alphabetical characters
- We are only comparing up to strlen(basedir) amount of characters in the test_file's path
- We are using strcmp to compare strings of differing lengths

## C) How you fixed the bug and why the fix was necessary
- We were originally using strncmp, where we compared only strlen(basedir) amount of characters to our test_file path. However, at line 99 (the last assert), the characters matched up perfectly, but the next character in our test_file was a "-" which technically should not be a safe path (recall that we are expecting a '/' instead). To fix this issue, we decided to instead compare the character after strlen(basedir) in the test_file path, which we mentioned earlier should be '/', by making this change, we were able to take care of this bug.
