#
my_local_repository = repository_rule(
	implementation=_impl,
	local=True,
	attrs={"path": attr.string(mandatory=True)}
)
