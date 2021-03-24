#!/usr/bin/env ruby

require 'fileutils'
require 'json'
require 'open3'

rootdir = Dir.pwd
target_binary = 'a.out'
input_case = '0001'
input_case_filename = "#{rootdir}/visualizer/in/#{input_case}.txt"
final_output_filename = '9999.txt'

exec_id = Time.new.strftime('%Y%m%d_%H%M%S') + "_#{input_case}"
dir = "public/results/#{exec_id}"
FileUtils.mkdir_p(dir)

Dir.chdir(dir)
logs = []
Open3.popen3("#{rootdir}/#{target_binary} < #{input_case_filename} > #{final_output_filename}") do |_, _, stderr, _|
  stderr.each do |line|
    puts line
    logs << line
  end
end

info = []
log_index = 0
Dir.chdir("#{rootdir}/visualizer")
Dir.glob("#{rootdir}/#{dir}/*.txt").sort.each do |fullpath|
  score = `cargo run --release --bin vis #{input_case_filename} #{fullpath}`.strip
  File.rename('out.svg', fullpath.sub('.txt', '.svg'))
  filename = fullpath.split('/').last
  timestamp = filename.to_i
  File.open(fullpath.sub('.txt', '.log'), 'w') do |f|
    while log_index < logs.count && logs[log_index].to_i <= timestamp
      f.write(logs[log_index])
      log_index += 1
    end
  end
  info << {rawdata: filename, image: filename.sub('.txt', '.svg'), log: filename.sub('.txt', '.log'), score: score}
end

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
metainfo = [exec_id] + metainfo
File.open("#{allinfo_filename}", 'w') { |f| f.write(JSON.pretty_generate(metainfo)) }
