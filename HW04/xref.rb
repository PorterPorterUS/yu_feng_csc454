#!/usr/bin/env ruby

class CodeLine 
    attr_accessor :address, :line, :func
    def initialize(addr, l, f = nil)
        self.address = addr
        self.line = l
        self.func = f
    end
end


class Xref 
    @name
    @first_line
    attr_accessor :assembly
    @subroutine
    def initialize(name, line)
        @name = name
        @assembly = Array.new
        @first_line = line;
        @subroutine = [];
    end
    def addSubroutine(func) 
        @subroutine << func
    end
    def showName
        puts @name
    end
    def showSubroutine
        puts @subroutine
    end
    def showLines
        @assembly.each { |code|
            print code.line
        }
    end
    def showAddrs
        @assembly.each { |code|
            puts code.address
        }
    end
end


######################################################################

objdump_output = []
obj_r, obj_io = IO.pipe
fork do
  system("objdump -d n_primes", out: obj_io, err: :out)
end
obj_io.close


objdump_head_match = /[0-9a-f]{16}.*\<(\p{Alnum}+?[\p{Alnum}_-]+)\>\:/
objdump_empty_line = /^[[:space:]]*$/
objdump_func_call  = /[\s]+([0-9a-f]{6})\:\s+e8\s+[[0-9a-f]{2}\s]{4}.+\<(\p{Alnum}+?[\p{Alnum}_-]+)\>.*/
objdump_asse_code  = /[\s]+([0-9a-f]{6})\:[\s]+[[0-9a-f]{2}\s]+.*/
objdump_dummy_head = /[0-9a-f]{16}.*\<frame_dummy\>:/

curr = nil
ifAdd = false

obj_r.each_line { |l| 
    if objdump_head_match.match(l) then
        curr =  Xref.new(Regexp.last_match(1), l)
        objdump_output << curr;
        ifAdd = true;
    elsif objdump_empty_line.match(l) then
        ifAdd = false
    else
        if objdump_func_call.match(l) then 
            curr.assembly << CodeLine.new(Regexp.last_match(1), 
                                    l, Regexp.last_match(2)) if ifAdd
            curr.addSubroutine(Regexp.last_match(2))
        elsif objdump_asse_code.match(l) then
            curr.assembly << CodeLine.new(Regexp.last_match(1), l) if ifAdd
        end
    end
    objdump_output.clear() if objdump_dummy_head.match(l)

}


total_assembly = {}
objdump_output.each { |e|
    e.showName
    e.showSubroutine
    e.showLines
    e.assembly.each { |l|
        total_assembly[l.address] = [l.line, l.func]
    }
}

# objdump_output.each { |e|
#     e.showName
#     e.showAddrs
# }

p total_assembly



######################################################################

class CodeMap
    attr_accessor :source_map, :path, :lines, :codes, :assembly_map
    @min
    @max
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
        if @lines[lno].nil? then 
            @lines[lno] = []
        end
        @lines[lno] << addr 
        @max = @max > lno ? @max : lno
    end
    def construct
        total = File.readlines(@path).size
        tmp = {}
        prev = ''
        (1..total).each { |i|
            if i < @min then
                tmp[i] = @lines[@min]
            elsif i > @max then
                tmp[i] = @lines[@max]
            else
                if !lines[i].nil? then
                    tmp[i] = lines[i]
                    prev = lines[i]
                else 
                    tmp[i] = prev
                end
            end
        }
        @lines = tmp
    end
    def read_file
        @codes = []
        File.open(@path).each { |l|
            @codes << l
        }
    end
    def match_assembly(total_assembly)
        @assembly_map = {}
        lines.each { |k,v|
            v.each { |a|
                if @assembly_map[a].nil? then
                    @assembly_map[a] = [total_assembly[a], k]
                else 
                    @assembly_map[a] << k
                end
            }
            
        }

    end
end

######################################################################

dwarf_output = []
dwarf_r, dwarf_io = IO.pipe
fork do
  res = system("~cs254/bin/dwarfdump -l n_primes", out: dwarf_io, err: :out)
end

dwarf_io.close

debug_matcher = /^0x00([0-9a-f]{6})\s+\[\s+([0-9]+)\,\s+([0-9]+)\]\s+([A-Z]+.*)$/
uri_matcher = /^(.*)uri\:[\s]+\"(.+)\".*$/

curr = nil

dwarf_r.each { |l|
    puts l
    if debug_matcher.match(l) then
        addr = Regexp.last_match(1)
        lno = Regexp.last_match(2).to_i
        col = Regexp.last_match(3).to_i
        info = Regexp.last_match(4)
        puts info
        if (uri_matcher.match(info)) then
            sig = Regexp.last_match(1).strip().split(' ')
            path = Regexp.last_match(2)
            curr = CodeMap.new(path, addr, lno, col, sig)
            dwarf_output << curr
        else 
            sig = info.strip().split(' ')
            curr.add_addr(lno, addr)
            curr.source_map[addr] = [lno, col, sig]
        end
    end
}


dwarf_output.each { |e|
    e.construct
    e.read_file
    puts e.lines
    puts e.codes
}

dwarf_output.each { |e|
    e.match_assembly(total_assembly)
    e.assembly_map.each { |l|
        p l
    }
}


