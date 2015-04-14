/* -*- Mode: C; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8; c-indent-level: 8 -*- */
/*
 *  Copyright (C) 2004 Red Hat, Inc.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2, or (at your option)
 *  any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

#include "config.h"

#include "ev-document-find.h"

G_DEFINE_INTERFACE (EvDocumentFind, ev_document_find, 0)

static void
ev_document_find_default_init (EvDocumentFindInterface *klass)
{
}

/**
 * ev_document_find_find_text:
 * @document_find: an #EvDocumentFind
 * @page: an #EvPage
 * @text: text to find
 * @case_sensitive: whether to match the string case
 *
 * Returns: (transfer full) (element-type EvRectangle): a list of results
 */
GList *
ev_document_find_find_text (EvDocumentFind *document_find,
			    EvPage         *page,
			    const gchar    *text,
			    gboolean        case_sensitive)
{
	EvDocumentFindInterface *iface = EV_DOCUMENT_FIND_GET_IFACE (document_find);

	return iface->find_text (document_find, page, text, case_sensitive);
}

/**
 * ev_document_find_find_text_with_options:
 * @document_find: an #EvDocumentFind
 * @page: an #EvPage
 * @text: text to find
 * @options: a set of #EvFindOptions
 *
 * Returns: (transfer full) (element-type EvRectangle): a list of results
 */
GList *
ev_document_find_find_text_with_options (EvDocumentFind *document_find,
					 EvPage         *page,
					 const gchar    *text,
					 EvFindOptions   options)
{
	EvDocumentFindInterface *iface = EV_DOCUMENT_FIND_GET_IFACE (document_find);

	printf("FIND TEXT (options): %s\n", text);
	GError **error = NULL;
	glong ucs4_len;
	gunichar *ucs4;
	ucs4 = g_utf8_to_ucs4 (text, -1, NULL, &ucs4_len, error);

	if(g_unichar_validate(text[0]))
		printf("char %c validated from input\n", text[0]);

	if(g_utf8_validate(text, -1, NULL))
		printf("utf8 validated: %s\n", text);

	if(g_unichar_validate(ucs4[0]))
		printf("char %c validated!\n", ucs4[0]);

	printf("---- UTF8 text received (string): %s\n", text);
	printf("---- UCS4 text converted (string): %s\n", (char*) ucs4);

	if (iface->find_text_with_options)
		return iface->find_text_with_options (document_find, page, text, options);

	return ev_document_find_find_text (document_find, page, text, options & EV_FIND_CASE_SENSITIVE);
}

EvFindOptions
ev_document_find_get_supported_options (EvDocumentFind *document_find)
{
	EvDocumentFindInterface *iface = EV_DOCUMENT_FIND_GET_IFACE (document_find);

	if (iface->get_supported_options)
		return iface->get_supported_options (document_find);
	return 0;
}
