# frozen_string_literal: true

source "https://rubygems.org"
git_source(:github) do |repo_name|
  "https://github.com/#{repo_name}.git"
end

gemspec

gem "liquid", github: "Shopify/liquid", ref: "master"

group :test do
  gem "rubocop", "~> 1.24.1", require: false
  gem "rubocop-performance", "~> 1.13.2", require: false
  gem "rubocop-shopify", "~> 2.4.0", require: false
  gem "spy", "0.4.1"
  gem "benchmark-ips"
  gem "ruby_memcheck"
end

group :development do
  gem "byebug"
end

gem "ruby-lsp", "~> 0.3.7", :group => :development
