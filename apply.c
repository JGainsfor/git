	git_config(git_default_config, NULL);
/*
 * This represents a "patch" to a file, both metainfo changes
 * such as creation/deletion, filemode and content changes represented
 * as a series of fragments.
 */
struct patch {
	char *new_name, *old_name, *def_name;
	unsigned int old_mode, new_mode;
	int is_new, is_delete;	/* -1 = unknown, 0 = false, 1 = true */
	int rejected;
	unsigned ws_rule;
	int lines_added, lines_deleted;
	int score;
	int extension_linenr; /* first line specifying delete/new/rename/copy */
	unsigned int is_toplevel_relative:1;
	unsigned int inaccurate_eof:1;
	unsigned int is_binary:1;
	unsigned int is_copy:1;
	unsigned int is_rename:1;
	unsigned int recount:1;
	unsigned int conflicted_threeway:1;
	unsigned int direct_to_threeway:1;
	unsigned int crlf_in_old:1;
	struct fragment *fragments;
	char *result;
	size_t resultsize;
	char old_sha1_prefix[41];
	char new_sha1_prefix[41];
	struct patch *next;

	/* three-way fallback result */
	struct object_id threeway_stage[3];
};

static char *find_name_gnu(struct apply_state *state,
			   const char *def,
	 * http://marc.info/?l=git&m=112927316408690&w=2
	if (state->root.len)
		strbuf_insert(&name, 0, state->root.buf, state->root.len);
static char *find_name_common(struct apply_state *state,
	if (state->root.len) {
		char *ret = xstrfmt("%s%.*s", state->root.buf, len, start);
static char *find_name(struct apply_state *state,
		char *name = find_name_gnu(state, line, def, p_value);
	return find_name_common(state, line, def, p_value, NULL, terminate);
static char *find_name_traditional(struct apply_state *state,
		char *name = find_name_gnu(state, line, def, p_value);
		return find_name_common(state, line, def, p_value, NULL, TERM_TAB);
	return find_name_common(state, line, def, p_value, line + len, 0);
	name = find_name_traditional(state, nameline, NULL, 0);
		name = find_name_traditional(state, second, NULL, state->p_value);
		name = find_name_traditional(state, first, NULL, state->p_value);
		first_name = find_name_traditional(state, first, NULL, state->p_value);
		name = find_name_traditional(state, second, first_name, state->p_value);
static int gitdiff_hdrend(struct apply_state *state,
static int gitdiff_verify_name(struct apply_state *state,
		*name = find_name(state, line, NULL, state->p_value, TERM_TAB);
		another = find_name(state, line, NULL, state->p_value, TERM_TAB);
static int gitdiff_oldname(struct apply_state *state,
static int gitdiff_newname(struct apply_state *state,
static int gitdiff_oldmode(struct apply_state *state,
static int gitdiff_newmode(struct apply_state *state,
static int gitdiff_delete(struct apply_state *state,
static int gitdiff_newfile(struct apply_state *state,
static int gitdiff_copysrc(struct apply_state *state,
	patch->old_name = find_name(state, line, NULL, state->p_value ? state->p_value - 1 : 0, 0);
static int gitdiff_copydst(struct apply_state *state,
	patch->new_name = find_name(state, line, NULL, state->p_value ? state->p_value - 1 : 0, 0);
static int gitdiff_renamesrc(struct apply_state *state,
	patch->old_name = find_name(state, line, NULL, state->p_value ? state->p_value - 1 : 0, 0);
static int gitdiff_renamedst(struct apply_state *state,
	patch->new_name = find_name(state, line, NULL, state->p_value ? state->p_value - 1 : 0, 0);
static int gitdiff_similarity(struct apply_state *state,
static int gitdiff_dissimilarity(struct apply_state *state,
static int gitdiff_index(struct apply_state *state,
	if (!ptr || ptr[1] != '.' || 40 < ptr - line)
	memcpy(patch->old_sha1_prefix, line, len);
	patch->old_sha1_prefix[len] = 0;
	if (40 < len)
	memcpy(patch->new_sha1_prefix, line, len);
	patch->new_sha1_prefix[len] = 0;
static int gitdiff_unrecognized(struct apply_state *state,
static const char *skip_tree_prefix(struct apply_state *state,
	if (!state->p_value)
	nslash = state->p_value;
static char *git_header_name(struct apply_state *state,
		cp = skip_tree_prefix(state, first.buf, first.len);
			cp = skip_tree_prefix(state, sp.buf, sp.len);
		cp = skip_tree_prefix(state, second, line + llen - second);
	name = skip_tree_prefix(state, line, llen);
			np = skip_tree_prefix(state, sp.buf, sp.len);
			second = skip_tree_prefix(state, name + len + 1,
static int check_header_line(struct apply_state *state, struct patch *patch)
			     patch->extension_linenr, state->linenr);
		patch->extension_linenr = state->linenr;
/* Verify that we recognize the lines following a git header */
static int parse_git_header(struct apply_state *state,
			    const char *line,
			    int len,
			    unsigned int size,
			    struct patch *patch)
	patch->def_name = git_header_name(state, line, len);
	if (patch->def_name && state->root.len) {
		char *s = xstrfmt("%s%s", state->root.buf, patch->def_name);
	state->linenr++;
	for (offset = len ; size > 0 ; offset += len, size -= len, line += len, state->linenr++) {
			int (*fn)(struct apply_state *, const char *, struct patch *);
			res = p->fn(state, line + oplen, patch);
			if (check_header_line(state, patch))
				return offset;
			int git_hdr_len = parse_git_header(state, line, len, size, patch);
			if (!patch->old_name && !patch->new_name) {
				if (!patch->def_name) {
					error(Q_("git diff header lacks filename information when removing "
							"%d leading pathname component (line %d)",
							"git diff header lacks filename information when removing "
							"%d leading pathname components (line %d)",
							state->p_value),
						     state->p_value, state->linenr);
					return -128;
				}
				patch->old_name = xstrdup(patch->def_name);
				patch->new_name = xstrdup(patch->def_name);
			}
			if ((!patch->new_name && !patch->is_delete) ||
			    (!patch->old_name && !patch->is_new)) {
				error(_("git diff header lacks filename information "
					     "(line %d)"), state->linenr);
				return -128;
			}
			patch->is_toplevel_relative = 1;
	if (!deleted && !added)
		patch->ws_rule = whitespace_rule(patch->new_name
						 ? patch->new_name
						 : patch->old_name);
		SWAP(p->old_sha1_prefix, p->new_sha1_prefix);
		die("BUG: caller miscounted postlen: asked %d, orig = %d, used = %d",
	 * full 40-byte textual SHA1 for old and new, at least for now.
	if (strlen(patch->old_sha1_prefix) != 40 ||
	    strlen(patch->new_sha1_prefix) != 40 ||
	    get_oid_hex(patch->old_sha1_prefix, &oid) ||
	    get_oid_hex(patch->new_sha1_prefix, &oid))
		if (strcmp(oid_to_hex(&oid), patch->old_sha1_prefix))
	get_oid_hex(patch->new_sha1_prefix, &oid);
	if (has_sha1_file(oid.hash)) {
		result = read_sha1_file(oid.hash, &type, &size);
				     patch->new_sha1_prefix, name);
		if (strcmp(oid_to_hex(&oid), patch->new_sha1_prefix))
				name, patch->new_sha1_prefix, oid_to_hex(&oid));
		result = read_sha1_file(oid->hash, &type, &sz);
	if (checkout_entry(ce, &costate, NULL) || lstat(ce->name, st))
static int verify_index_match(const struct cache_entry *ce, struct stat *st)
	return ce_match_stat(ce, st, CE_MATCH_IGNORE_VALID|CE_MATCH_IGNORE_SKIP_WORKTREE);
static int three_way_merge(struct image *image,
			  &their_file, "theirs", NULL);
		die("BUG: patch to %s is not a creation", patch->old_name);
	pos = cache_name_pos(name, strlen(name));
	ce = active_cache[pos];
		if (checkout_target(&the_index, ce, &st))
	if (verify_index_match(ce, &st))
	else if (get_oid(patch->old_sha1_prefix, &pre_oid) ||
	status = three_way_merge(image, patch->new_name,
		int pos = cache_name_pos(old_name, strlen(old_name));
		*ce = active_cache[pos];
			if (checkout_target(&the_index, *ce, st))
		if (!state->cached && verify_index_match(*ce, st))
	    cache_name_pos(new_name, strlen(new_name)) >= 0 &&
			ce = cache_file_exists(name->buf, name->len, ignore_case);
		return read_cache_from(state->index_file);
		return read_cache();
	pos = cache_name_pos(path, strlen(path));
	oidcpy(oid, &active_cache[pos]->oid);
	    preimage[sizeof(heading) + GIT_SHA1_HEXSZ - 1] == '\n' &&
	    starts_with(preimage + sizeof(heading) - 1, p->old_sha1_prefix))
	return get_oid_hex(p->old_sha1_prefix, oid);
/* Build an index that contains the just the files needed for a 3way merge */
	static struct lock_file lock;
		} else if (!get_oid_blob(patch->old_sha1_prefix, &oid)) {
		ce = make_cache_entry(patch->old_mode, oid.hash, name, 0, 0);
			free(ce);
	if (state->update_index) {
		if (remove_file_from_cache(patch->old_name) < 0)
	unsigned ce_size = cache_entry_size(namelen);

	if (!state->update_index)
		return 0;
	ce = xcalloc(1, ce_size);
	if (S_ISGITLINK(mode)) {
			free(ce);
		       return error(_("corrupt patch for submodule %s"), path);
				free(ce);
			fill_stat_cache_info(ce, &st);
			free(ce);
	if (add_cache_entry(ce, ADD_CACHE_OK_TO_ADD) < 0) {
		free(ce);
static int try_create_file(const char *path, unsigned int mode, const char *buf, unsigned long size)
	if (convert_to_working_tree(path, buf, size, &nbuf)) {
	res = try_create_file(path, mode, buf, size);
		res = try_create_file(path, mode, buf, size);
			res = try_create_file(newpath, mode, buf, size);
	unsigned ce_size, mode;
	ce_size = cache_entry_size(namelen);
	remove_file_from_cache(patch->new_name);
		ce = xcalloc(1, ce_size);
		if (add_cache_entry(ce, ADD_CACHE_OK_TO_ADD) < 0) {
			free(ce);
	else
		rerere(0);
	state->update_index = state->check_index && state->apply;
			hold_locked_index(&state->lock_file, LOCK_DIE_ON_ERROR);
		exit(1);
		res = write_locked_index(&the_index, &state->lock_file, COMMIT_LOCK);
			0, apply_option_parse_exclude },
			0, apply_option_parse_include },