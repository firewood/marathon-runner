#!/usr/bin/env ruby

require 'fileutils'
require 'json'

rootdir = Dir.pwd
target_binary = 'a.out'
input_case = '0001'
input_case_filename = "#{rootdir}/visualizer/in/#{input_case}.txt"

exec_id = Time.new.strftime('%Y%m%d_%H%M%S') + "_#{input_case}"
dir = "public/results/#{exec_id}"
FileUtils.mkdir_p(dir)

Dir.chdir(dir)
`#{rootdir}/#{target_binary} < #{input_case_filename}`

info = []
Dir.chdir("#{rootdir}/visualizer")
Dir.glob("#{rootdir}/#{dir}/*.txt") do |filename|
  score = `cargo run --release --bin vis #{input_case_filename} #{filename}`.strip
  File.rename('out.svg', filename.sub('.txt', '.svg'))
  filename = filename.split('/').last
  info << {rawdata: filename, image: filename.sub('.txt', '.svg'), score: score}
end
puts info

info_filename = "#{dir}/info.json"
File.open("#{rootdir}/#{info_filename}", 'w') { |f| f.write(JSON.pretty_generate(info)) }

allinfo_filename = "#{rootdir}/public/results/all.json"
metainfo = []
begin
  File.open(allinfo_filename, 'r') do |f|
    metainfo = JSON.parse(f.read)
  end
rescue => error
end
metainfo << exec_id
File.open("#{allinfo_filename}", 'w') { |f| f.write(JSON.pretty_generate(metainfo)) }

