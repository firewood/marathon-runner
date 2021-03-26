#!/usr/bin/env ruby

path = ARGV.first
if !path || !Dir.exists?(path)
  puts "Path not found"
  exit 1
end

seq = 1
Dir.glob("#{path}/*.svg").sort.each do |fullpath|
  `convert -density 12 -resize 800x800 #{fullpath} #{path}/#{sprintf('%04d', seq)}.png`
  seq += 1
end

`ffmpeg -r 2 -i #{path}/%04d.png -vcodec libx264 -pix_fmt yuv420p -y #{path}/out.mp4`
