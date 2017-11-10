#!/usr/bin/env ruby
require 'cgi'

# execute gcc to compile the code into out
myprogram = ARGV[0]

# store the info for each line in source code
# for every line of Assembly code, it has the attributes 
# codeLine => [line_content, jmp_addr/calling_addr, func]
# if no jmp_addr/calling_addr or func, put "nil".

class CodeLine 
    attr_accessor :address, :line, :jmp_addr, :func
    def initialize(addr, l, ad = nil, f = nil)
        self.address = addr
        self.line = l
        self.jmp_addr = ad
        self.func = f
    end
end

# this class store Assembly codes by functions, for each functions, 
# this class will store the first_addr of the function, name, subroutine
# and mapping functions to map each line of assembly code. 

class FuncAssembly
    @first_line
    @name
    attr_accessor :assembly, :first_addr
    @subroutine
    def initialize(name, line)
        @name = name
        @assembly = Array.new
        @first_line = line;
        @first_addr = line.split(' ')[0][10..16]
        @subroutine = [];
    end
    def add_subroutine(addr, func=nil) 
        @subroutine << [addr, func]
    end
    def show_name
        puts @name
    end
    def get_name
        @name
    end
    def show_subroutine
        puts @subroutine
    end
    def get_subroutine
        @subroutine
    end
    def show_lines
        @assembly.each { |code|
            print code.line
        }
    end
    def show_addrs
        @assembly.each { |code|
            puts code.address
        }
    end
end

# This class is to store the information from dwarfdump and cross match 
# with the assembly code. 
# :source_map is use to store the information from dwarfdump
# the map key is [addr] => [line_num, col_num, some_info]
# :codes is the actual sourse code map by [line_num] => code_text
# :assembly_map is mapping each aseembly addr with correspoding code_line, 
# and assembly code.
# Information stores in assembly map is like 
# [addr] ==> [line_num1, line_num2, ... , [assembly_code, jmp_addr, func_name]]
#  rest of attributes are used for easy to index assembly code with sourse code.
class CodeMap
    # @lines [lno] => [addr1, addr2, ...]
    attr_accessor :source_map, :path, :lines, :codes, :assembly_map, :min
    @max
    @total_num_lines
    def initialize(path, addr, lno, col, sig)
        @min = lno
        @max = lno
        @path = path
        @lines = {}
        @lines[lno] = []
        @lines[lno] << addr
        @source_map = {}
        @source_map[addr] = [lno, col, sig]
    end
    def add_addr(lno, addr)
        @lines[lno] = [] if @lines[lno].nil?
        @lines[lno] << addr 
        @max = lno if (@max < lno)
        @min = lno if (@min > lno)
    end
    # # this will match 
    # def construct
    #     tmp = {}
    #     prev = ''
    #     (@min..total).each { |i|
    #         if i > @max then
    #             tmp[i] = @lines[@max]
    #         else
    #             if !lines[i].nil? then
    #                 tmp[i] = lines[i]
    #                 prev = lines[i]
    #             else 
    #                 tmp[i] = prev
    #             end
    #         end
    #     }
    #     @lines = tmp
    # end
    def read_file
        @codes = [@path]
        @total_num_line = File.readlines(@path).size
        File.open(@path).each { |l|
            @codes << l
        }
    end
    def match_assembly
        @assembly_map = {}
        lines.each { |lno, assems|
            assems.each { |assem|
                if @assembly_map[assem].nil? then
                    @assembly_map[assem] = [lno]
                else 
                    @assembly_map[a] << lno
                end
            }
            
        }
    end
    def add_func(total_assembly)
        addrs = [];
        assembly_map.each_key { |addr|
            if !total_assembly[addr].nil? then
                addrs << addr
            end
        }
        addrs.each { |addr|
            total_assembly[addr].each { |k, v|
                if assembly_map[k].nil? then
                    assembly_map[k] = [v]
                else
                    assembly_map[k] << v 
                end
            }
        }
    end
end


######################################################################
#   This sections is to obtains the information from objdump         # 
######################################################################

# read lines from objdump
obj_r, obj_io = IO.pipe
fork do
  system("objdump -d #{myprogram}", out: obj_io, err: :out)
end
obj_io.close

# regex for matching purposes.
# This is to match the head line of each objdump function.
objdump_head_match = /[0-9a-f]{16}.*\<(\p{Alnum}+?[\p{Alnum}_-]+)\>\:/
# match empty line
objdump_empty_line = /^[[:space:]]*$/
# match function call
objdump_func_call  = /[\s]+([0-9a-f]{6})\:[\s]+([0-9a-f]{2}\s){5}(\s|callq)*([0-9a-f]{6}).*\<(\p{Alnum}+?[\p{Alnum}_]+)\>.*/
# match jmp instructions
objdump_loc_jump = /[\s]+([0-9a-f]{6})\:[\s]+([0-9a-f]{2}\s){2,}[\s]*j[a-z]*\s*([0-9a-f]{6}).*\<(\p{Alnum}+?\S+)\>.*\n/
# match regular assembly code
objdump_asse_code  = /[\s]+([0-9a-f]{6})\:[\s]+([0-9a-f]{2}\s)+.*/
# match <dummy_head>, so it will start to store the information of 
# program, not stdlib.
objdump_dummy_head = /[0-9a-f]{16}.*\<frame_dummy\>:/


# store the FuncAssembly class
objdump_funs = []
curr = nil
ifAdd = false

# start to match the content in objdump
obj_r.each_line { |l| 
    # to match head line such as: 00000000004007d0 <main>:
    if objdump_head_match.match(l) then
        # grep the function head line
        curr =  FuncAssembly.new(Regexp.last_match(1), l)
        objdump_funs << curr;
        ifAdd = true;
        # stop add sequential lines if meet a blank line.
    elsif objdump_empty_line.match(l) then
        ifAdd = false
    else
        # match function call but not internal function call e.g. start with "_"
        if ifAdd && objdump_func_call.match(l) then
            curr.assembly << CodeLine.new(Regexp.last_match(1), 
                    l, Regexp.last_match(4), Regexp.last_match(5))
            curr.add_subroutine(Regexp.last_match(4), Regexp.last_match(5))
        elsif ifAdd && objdump_loc_jump.match(l) && ifAdd then 
            curr.assembly << CodeLine.new(Regexp.last_match(1), 
                                    l, Regexp.last_match(3))
            curr.add_subroutine(Regexp.last_match(3))
        elsif ifAdd && objdump_asse_code.match(l) then
            curr.assembly << CodeLine.new(Regexp.last_match(1), l)
        end
    end
    # objdump_funs.clear() if objdump_dummy_head.match(l)

}

######################################################################
#         Actual run dwarfdump and get inforamtions from it          #
######################################################################

dwarf_output = {}
dwarf_r, dwarf_io = IO.pipe
fork do
  res = system("~cs254/bin/dwarfdump -l #{myprogram}", out: dwarf_io, err: :out)
end

dwarf_io.close

######################################################################
#   This sections is to obtains the information from dwarfdump       # 
######################################################################

debug_matcher = /^0x00([0-9a-f]{6})\s+\[\s+([0-9]+)\,\s+([0-9]+)\]\s+([A-Z]+.*)$/
uri_matcher = /^(.*)uri\:[\s]+\"(.+)\".*$/

curr = nil

dwarf_r.each { |l|
    puts l
    # match the debug_line in dwarfdump, so that we will start to read info.
    if debug_matcher.match(l) then
        addr = Regexp.last_match(1)
        lno = Regexp.last_match(2).to_i
        col = Regexp.last_match(3).to_i
        info = Regexp.last_match(4)
        if (uri_matcher.match(info)) then
            sig = Regexp.last_match(1).strip().split(' ')
            path = Regexp.last_match(2)
            if dwarf_output[path].nil? then
                curr = CodeMap.new(path, addr, lno, col, sig)
                dwarf_output[path] = curr
            else
                curr = dwarf_output[path]
            end
        else 
            sig = info.strip().split(' ')
            curr.add_addr(lno, addr)
            curr.source_map[addr] = [lno, col, sig]
        end
    end
}

puts "######################################################################"
puts "THIS BELOW IS DWARF_CODE_LINE_DEBUG_INFO"
puts "######################################################################"
dwarf_output.each { |path, source|
    p path
    source.source_map.each { |pair|
        p pair
    }
}

own_code_addr = {}
lib_path = ".*/include/.*"

dwarf_output.each { |path, source|
    if !path.match(lib_path) then
        source.lines.each { |key, lines|
            lines.sort
        }
        source.source_map.each_key { |addr| 
            own_code_addr[addr] = true
        }
        # e.construct
        source.read_file
        puts source.lines
        puts source.codes
    else 
        dwarf_output.delete(path)
    end
}

# After extracted useful information for objdump, build an assembly code 
# map to map each assembly code by addr.
# and also find the addr of main function.
objdump_funs.each { |func|
    # actual useful code 
    objdump_funs.delete(func) if own_code_addr[func.first_addr].nil?
}


puts "######################################################################"
puts "          THIS BELOW IS ASSEM_EXTRACT_DEBUG_INFO"
puts "######################################################################"
main_func_adrr = ""
total_assembly = {}
objdump_funs.each { |func|
    # debug purpose 
    func.show_name
    func.show_lines
    # actual useful code 
    main_func_adrr = func.first_addr if func.get_name == "main"
    total_assembly[func.first_addr] = {}
    func.assembly.each { |l|
        total_assembly[func.first_addr][l.address] = [l.line, l.jmp_addr, l.func]
    }
}

puts "######################################################################"
puts "                THIS BELOW IS FUNC_ASSEM_DEBUG_INFO"
puts "######################################################################"
# dubug purpose
total_assembly.each { |first_addr, func_assem|
    p first_addr
    func_assem.each { |addr_and_info|
        p addr_and_info
    }
}
puts "######################################################################"
puts "THIS BELOW IS DWARF_ASSEMBLY_MATCH_DEBUG_INFO"
puts "######################################################################"
dwarf_output.each { |k, e|
    e.match_assembly
    e.add_func(total_assembly)
    e.assembly_map.each { |l|
        p l
    }
}


all_code = []
all_asse = []
href = {}
href_index = 0
check_line = {}
check_index = 0
dwarf_output.each { |k, e|
    assembly_text = ''
    code_text = ''
    lead_cnt = 1
    asse_cnt = 1
    code_cnt = 1
    lines_set = {}
    while code_cnt < e.min
        code_text += "[#{code_cnt}, X]\t#{e.codes[code_cnt]}"
        all_code << "[#{code_cnt}, X]\t#{e.codes[code_cnt]}".chop
        code_cnt += 1
        check_index += 1
    end
    lead_cnt = lead_cnt < code_cnt ? code_cnt : lead_cnt
    e.assembly_map.keys.sort.each { |addr|
        curr_cnt = lead_cnt
        need_write = true
        e.assembly_map[addr].each { |i|
            if i.class == Integer && need_write then
                while code_cnt < curr_cnt 
                    code_text += "[#{code_cnt}, X]\t\n"
                    all_code << "[#{code_cnt}, X]"
                    code_cnt += 1
                    check_index += 1
                end
                code_text += "[#{code_cnt}, #{i}]\t#{e.codes[i]}"
                all_code << "[#{code_cnt}, #{i}]\t#{e.codes[i]}".chop
                if lines_set.key?(i) then
                    need_write = false 
                    check_line[check_index] = true
                end
                lines_set[i] = true
                code_cnt += 1
                check_index += 1
                lead_cnt = lead_cnt < code_cnt ? code_cnt : lead_cnt
            elsif i.class != Integer then
                while asse_cnt < curr_cnt
                    assembly_text += "[#{asse_cnt}]\t\n"
                    all_asse << ""
                    asse_cnt += 1
                    href_index += 1
                end
                assembly_text += "[#{asse_cnt}]\t#{i[0]}"
                all_asse << "#{i[0]}".chop
                if (!i[1].nil?) then
                    href[href_index] = i[1]
                    href[i[1]] = true
                end
                asse_cnt += 1
                href_index += 1
                lead_cnt = lead_cnt < asse_cnt ? asse_cnt : lead_cnt
            end
        }

    }
    while code_cnt < lead_cnt 
        code_text += "[#{code_cnt}, X]\t\n"
        all_code << "[#{code_cnt}, X]"
        code_cnt += 1
        check_index += 1
    end
    while asse_cnt < lead_cnt 
        assembly_text += "[#{asse_cnt}]\t\n"
        all_asse << ""
        href_index += 1
        asse_cnt += 1
    end
    print assembly_text
    puts "==================================="
    print code_text
    puts "***********************************"
}

p href

html_text = "<!DOCTYPE html>
<html>
<head>
    <title>CSC 454 HW04</title>
    <style type=\"text/css\">

        * { 
            font-family: monospace; 
            line-height: 1.5em;
        }

        table {
            width: 100%;
        }

        td
        {
            padding: 8px;
            vertical-align: bottom;
            width: 50%;
        }

        th
        {
            border: 1px solid black;
        }

        .grey {
            color: #888
        }

    </style>
</head>
<body>
    <table>
"


all_code.each_index { |i|
    html_text += "      <tr>\n"
    if check_line[i] then
        html_text += "          <td class=\"grey\">\n"
    else
        html_text += "          <td>\n"
    end
    html_text += "              " + CGI.escapeHTML(all_code[i]).gsub(/ /, "&nbsp;") + "<br>\n"
    html_text += "          </td>\n"
    html_text += "          <td>\n"

    if (all_asse[i].length > 1) then
        addr = all_asse[i].split(":")[0].strip
    end
    if href[addr] || addr == main_func_adrr then
        html_text += "          <a name=\""+ addr +"\">\n"
        html_text += "              " + CGI.escapeHTML(all_asse[i]).gsub(/ /, "&nbsp;") + "<br>\n"
        html_text += "          </td>\n"
    elsif !href[i].nil? then 
        html_text += "          <a href=\"#"+ href[i] +"\">"
        html_text += "              " + CGI.escapeHTML(all_asse[i]).gsub(/ /, "&nbsp;") + "</a> <br>\n"
        html_text += "          </td>\n"
    else 
        html_text += "              " + CGI.escapeHTML(all_asse[i]).gsub(/ /, "&nbsp;") + "<br>\n"
        html_text += "          </td>\n"
    end
    
    html_text += "      </tr>\n"
}

html_text += "
    </table>
</body>
</html>
"

File.write('./main.html', html_text)

html_index = "<!DOCTYPE html>
<html>
<head>
    <title>CSC 454 HW04</title>
    <style type=\"text/css\">

        * { 
            font-family: monospace; 
            line-height: 1.5em;
        }

        table {
            width: 100%;
        }

        td
        {
            padding: 8px;
            vertical-align: bottom;
            width: 50%;
        }

        th
        {
            border: 1px solid black;
        }

        .grey {
            color: #888
        }

    </style>
</head>
<body>
    <table>
      <tr>
          <td>
          <a href=\"./main.html##{main_func_adrr}\"> main </a> <br>
          </td>
      </tr>
    </table>
</body>
</html>
"
File.write('./index.html', html_index)