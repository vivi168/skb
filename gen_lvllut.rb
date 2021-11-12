#!/usr/bin/env ruby

def filenames(files, fmt: :pc)
  files.each do |file|
    label = File.basename(file, '.txt')

    if fmt == :pc
      puts "    \"#{file}\","
    else
      filepath = file.gsub('/') { "\\\\" }.upcase
      puts "    \"\\\\#{filepath};1\","
    end
  end
end

path = File.expand_path(File.dirname(__FILE__))
files = Dir.glob('levels/level*.txt', base: path).sort

puts '#include "level.h"'
puts 'const char *level_LUT[LEVEL_COUNT] = {'
puts '#ifdef PCVER'
filenames(files)
puts '#else'
filenames(files, fmt: :psx)
puts '#endif'
puts '};'
