#
#  -- Here is the ruby implementation of combination;
#     To run the code, simply run: ruby combinations.rb k n;

#
def combination(num, final)
    combination(num, 1, final)
end

# This combination function is a recursive function to recursively call 
# itself, and then examine the returned list, several "if" statement is
# listed below:

# if:     the return list is empty, return a empty list, or if the 
#         combination number is larger the range of (start, end);
# elseif: the combination number is "1", just return all the number into
#         string as a list;
# elseif: the combination just equals to the range between the "start" 
#         and "end", then just simply return all the number in a single
#         string; 
# else:   add the additional number (if fits the critera) on each list;
def combinationThree(num, start, final) 
    # check the first "if" statement.
    if num > (final - start + 1)
        []
    # check the second statement, and 
    # return a list a single number string.
    elsif num == 1
        (start.to_s..final.to_s).to_a
    # if the combination number equals to (end - start),
    # then return a single string of numbers as a list.
    elsif num == (final - start + 1)
        line = start.to_s
        while start < final
            start += 1
            line << (" " + start.to_s)
        end
        [line]
    # make a for loop to call every sub combination function, and then,
    # if the sub-function return a actually list, add the frist element
    # into each string, such as:
    #     list = combination(1, 2, 3);  <-- return {"2", "3"};
    #     for loop add each string "1 ";
    #     so it will return {"1 2", "1 3"};
    # if the string.length == 0, then it won't add additional number. 
    else
        line_set = [];
        for i in ((start + 1)..final)
            for line in combinationThree(num - 1, i, final)
                if line.length > 0
                    line.insert(0, (i - 1).to_s + " ")
                    line_set << line
                end
            end
        end
        line_set
    end
end

k = ARGV[0].to_i
n = ARGV[1].to_i

# make a list to store the combination in string;
# print all the combination into concole;

puts combinationThree(k, 1, n)
