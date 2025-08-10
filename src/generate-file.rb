#!/usr/bin/env ruby
# frozen_string_literal: true

require 'fileutils'


class ClassCreator
  LICENSE = 'ISC'
  attr_reader :name
  attr_reader :namespace
  attr_reader :no_class
  attr_reader :c_only
  def initialize(name:, namespace: %i[ope], no_class: false, c_only: false)
    @name = name
    @namespace = namespace
    @no_class = no_class
    @c_only= c_only  end

  def camel_case
    @camel_case ||= name.split('_').map(&:capitalize).join
  end

  def header_guard
    @header_guard ||= "#{namespace.join('_').upcase}_#{name.upcase}_H"
  end

  def directory
    @relative_directory ||= File.join(namespace.map(&:to_s))
  end

  def header_path
    @header_path ||= File.join(directory, "#{name}.h")
  end

  def source_path
    @source_path ||= File.join(directory, c_only ? "#{name}.c" : "#{name}.cpp")
  end

  def body
    if c_only
      <<~END_SRC
        #ifdef __cplusplus
        extern "C" {
        #endif

        #ifdef __cplusplus
        } /* extern "C" */
        #endif
      END_SRC
    else
      <<~END_SRC
        namespace #{namespace.join('::')} {
        #{no_class ? '' : "class #{camel_case} final : public util::Object { };"}
        } /* namespace #{namespace.join('::')} */
      END_SRC
    end
  end

  def header
    @header ||= <<~END_SRC
      /* SPDX-License-Identifier: #{LICENSE} */

      #ifndef #{header_guard}
      #define #{header_guard}

      #include "euler/util/object.h"

      #{body}

      #endif /* #{header_guard} */

    END_SRC
  end

  def source
    @source ||= <<~END_SRC
      /* SPDX-License-Identifier: #{LICENSE} */

      #include "#{header_path}"

    END_SRC
  end

  def create
    FileUtils.mkdir_p(directory)
    puts "Writing #{header_path}..."
    File.write(header_path, header)
    puts "Writing #{source_path}..."
    File.write(source_path, source)
  end
end

if ARGV.size < 1 || ARGV.size > 2
  puts "Usage: make_class.rb [--no-class] <class_name>"
  exit
end

def to_snake_case(str)
  str.gsub(/([A-Z])/) { |m| "_#{m.downcase}" }.sub(/^_/, '')
end

arg = ARGV[0]

no_class = false
c_only = false

if arg == '--no-class'
  no_class = true
  arg = ARGV[1]
elsif arg == '--c-only'
  c_only= true
  arg = ARGV[1]
end

parts = arg.split('::')

namespace = parts[0..-2].map(&:to_sym) if parts.size > 1

args = {
  name: to_snake_case(parts[-1]),
  no_class: no_class,
  c_only: c_only,
}

args[:namespace] = namespace if namespace


cc = ClassCreator.new(**args)

cc.create