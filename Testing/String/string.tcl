load ./libWrapString.dll
stdstring s ""


# Create an instance of std::string.
stdstring s0 "FooBar"

# Call the C++ assignment operator to set it.
#s0 = "FooBar"

# Print the string's value:
puts [s0 c_str]

# Create another instance 
stdstring s1 "Hello, World!"
puts [s1 c_str]
puts [s1 find_first_of "," 0]
# Find the substring before the first comma.
set s2 [s1 substr 0 [s1 find_first_of "," 0]]
puts [$s2 c_str]

