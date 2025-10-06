#!/usr/bin/env ruby

require 'pathname'
require 'fileutils'
require 'tmpdir'

EXTERN_DIR = Pathname.new(__FILE__).dirname.expand_path
MRUBY_CONFIG = EXTERN_DIR.join('mruby-config.rb')
MRUBY = EXTERN_DIR.join('mruby')
PRESYM = EXTERN_DIR.join('mruby-presym.txt')
SOURCE_DIRS = %w[src build/host/mrblib build/host/mrbgems]

raise "Cannot find #{MRUBY_CONFIG}" unless MRUBY_CONFIG.exist?
raise "Cannot find #{MRUBY}" unless MRUBY.exist?

print 'Identifying required mruby source files...'

headers = []
sources = []

Dir.mktmpdir do |dir|
  dir = Pathname.new(dir)
  FileUtils.cp_r MRUBY_CONFIG, dir
  FileUtils.cp_r MRUBY, dir
  FileUtils.cp_r PRESYM ,dir
  Dir.chdir(dir.join('mruby')) do
    system("rake deep_clean > /dev/null", exception: true)
    system("MRUBY_CONFIG=#{MRUBY_CONFIG} rake all > /dev/null", exception: true)
    gems = dir.join('mruby/build/host/mrbgems').children
             .filter(&:directory?)
             .map(&:basename)
             .map(&:to_s)
    source_directories = [
      SOURCE_DIRS.map { |d| dir.join("mruby/#{d}") },
      gems.map { |g| dir.join('mruby/mrbgems', g.to_s) },
    ].flatten.map(&:expand_path)
    headers = dir.join('mruby/build').find
                .filter { |f| f.extname.end_with?(*%w[h hpp]) }
                .map { |f| f.relative_path_from(dir) }
    sources = source_directories.map { |d| Pathname.new(MRUBY.join(d))}
                .map { |f| f.find.to_a }.flatten
                .filter { |p| p.extname.end_with?(*%w[h hpp c cpp]) }
                .map { |f| f.relative_path_from(dir) }
  end
end

print " done\n"

EXTERN_DIR.join('mruby-headers.txt').open('w') do |fh|
  headers.each { |h| fh.puts h }
end

puts "Identified #{headers.count} C/C++ header files"

EXTERN_DIR.join('mruby-sources.txt').open('w') do |fh|
  sources.each { |s| fh.puts s }
end

puts "Identified #{sources.count} C/C++ source files"

