require 'pathname'

PRESYM_PATH = Pathname.new(__FILE__).dirname.join('presym')

MRuby::Build.new do |conf|
  conf.toolchain
  conf.gembox 'default'
  conf.enable_cxx_exception
  def self.define_rules
    super

    class << self.presym
      def scan(*args)
        presyms = super(*args)
        File.readlines(PRESYM_PATH, chomp: true).each do |line|
          presyms << line
        end
        # presyms << "extra_symbol1"
        # presyms << "extra_symbol2"
        # presyms << "$global_variable"
        # presyms << "\x01\x02\x03"
        presyms.uniq!
        presyms.sort_by! { |e| [e.bytesize, e] }
        presyms
      end
    end
  end
end
